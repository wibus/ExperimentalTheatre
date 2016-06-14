#include "PixelPrioritizer.h"

#include <chrono>

#include <CellarWorkbench/Misc/Log.h>

#include "ConvergentFilm.h"

using namespace cellar;


namespace prop3
{
    PixelPrioritizer::PixelPrioritizer() :
        _frameAvrgPriority(1.0)
    {

    }

    PixelPrioritizer::~PixelPrioritizer()
    {

    }

    void PixelPrioritizer::reset(
            const glm::ivec2& frameResolution,
            double blurVariance)
    {
        int halfSize = KERNEL_WIDTH / 2;
        for(int i=-halfSize; i<=halfSize; ++i)
        {
            int idx = i+halfSize;
            _gauss[idx] = glm::exp(-(i*i) / (2.0*blurVariance));
        }

        _frameAvrgPriority = 1.0;

        unsigned int pixelCount =
                frameResolution.x *
                frameResolution.y;
        if(pixelCount != _varBuff.size())
        {
            _varBuff.resize(pixelCount);
            _tmpBuff.resize(pixelCount);
        }
    }

    bool PixelPrioritizer::launchPrioritization(ConvergentFilm& film)
    {
//        using std::chrono::high_resolution_clock;
//        auto tStart = high_resolution_clock::now();

        std::vector<glm::dvec4>& refSampBuff = film._referenceFilm.sampleBuffer;
        std::vector<glm::dvec4>& rawSampBuff = film._sampleBuffer;
        std::vector<glm::dvec2>& rawVarBuff = film._varianceBuffer;
        std::vector<double>& prioBuff = film._priorityBuffer;
        glm::ivec2 frameResolution = film.frameResolution();
        size_t pixelCount = rawSampBuff.size();

        for(size_t p=0; p < pixelCount; ++p)
        {
            if(rawVarBuff[p].y > 0.0)
                _varBuff[p] = rawVarBuff[p].x / rawVarBuff[p].y;
            else
                _varBuff[p] = 1.0;
        }

        for(int j=0; j<frameResolution.y; ++j)
        {
            unsigned int lineBaseIdx = j*frameResolution.x;

            for(int i=0; i<frameResolution.x; ++i)
            {
                unsigned int idx = lineBaseIdx + i;

                _tmpBuff[idx] = _varBuff[idx];

                if(i - 1 >= 0)
                {
                    _tmpBuff[idx] = glm::max(_tmpBuff[idx], _varBuff[idx-1] * _gauss[1]);

                    if(i - 2 >= 0)
                    {
                        _tmpBuff[idx] = glm::max(_tmpBuff[idx], _varBuff[idx-2] * _gauss[0]);
                    }
                }

                if(i + 1 < frameResolution.x)
                {
                    _tmpBuff[idx] = glm::max(_tmpBuff[idx], _varBuff[idx+1] * _gauss[3]);

                    if(i + 2 < frameResolution.x)
                    {
                        _tmpBuff[idx] = glm::max(_tmpBuff[idx], _varBuff[idx+2] * _gauss[4]);
                    }
                }
            }
        }

        for(int j=0; j<frameResolution.y; ++j)
        {
            unsigned int lineBaseIdx = j*frameResolution.x;

            for(int i=0; i<frameResolution.x; ++i)
            {
                unsigned int idx = lineBaseIdx + i;

                double maxVar = _tmpBuff[idx];

                if(j - 1 >= 0)
                {
                    maxVar = glm::max(maxVar, _tmpBuff[idx-frameResolution.x] * _gauss[1]);

                    if(j - 2 >= 0)
                    {
                        maxVar = glm::max(maxVar, _tmpBuff[idx-2*frameResolution.x] * _gauss[0]);
                    }
                }

                if(j + 1 < frameResolution.y)
                {
                    maxVar = glm::max(maxVar, _tmpBuff[idx+frameResolution.x] * _gauss[3]);

                    if(j + 2 < frameResolution.y)
                    {
                        maxVar = glm::max(maxVar, _tmpBuff[idx+2*frameResolution.x] * _gauss[4]);
                    }
                }


                double compatibility = film.refShotCompatibility(idx);
                glm::dvec4 mixedSample = rawSampBuff[idx] +
                        refSampBuff[idx] * compatibility;

                prioBuff[idx] = film.toPriority(mixedSample, maxVar);
            }
        }

        _frameAvrgPriority = 0.0;
        for(const std::shared_ptr<Tile>& tile : film._tiles)
        {
            double tileMaxPriority = 0.0;
            for(int j=tile->minCorner().y; j < tile->maxCorner().y; ++j)
            {
                double tilePrioSum = 0.0;
                int index = j * frameResolution.x + tile->minCorner().x;
                for(int i=tile->minCorner().x; i < tile->maxCorner().x; ++i, ++index)
                {
                    tilePrioSum += prioBuff[index] * prioBuff[index];

                    if(tileMaxPriority < prioBuff[index])
                        tileMaxPriority = prioBuff[index];
                }

                _frameAvrgPriority += tilePrioSum;
            }

            tile->setTilePriority(tileMaxPriority);
        }
        _frameAvrgPriority = glm::sqrt(_frameAvrgPriority / pixelCount);

        if(film._colorOutput == Film::ColorOutput::PRIORITY)
        {
            unsigned int pixelCount =
                    frameResolution.x *
                    frameResolution.y;

            for(int i=0; i < pixelCount; ++i)
                film._colorBuffer[i] =
                    film.priorityToColor(
                        film._priorityBuffer[i]);
        }

//        auto tEnd = high_resolution_clock::now();
//        float us = (tEnd - tStart).count() / 1.0e3;
//        getLog().postMessage(new Message('I', false,
//            "Pixel Prioritization time (us) :\t " +
//                std::to_string(us),
//            "PixelPrioritizer"));
    }

    double PixelPrioritizer::averagePriority() const
    {
        return _frameAvrgPriority;
    }
}
