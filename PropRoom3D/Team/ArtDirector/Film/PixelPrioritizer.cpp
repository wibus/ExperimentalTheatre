#include "PixelPrioritizer.h"

#include <chrono>

#include <CellarWorkbench/Misc/Log.h>

#include "ConvergentFilm.h"

using namespace cellar;


namespace prop3
{
    PixelPrioritizer::PixelPrioritizer() :
        _frameMaxPriority(1.0)
    {

    }

    PixelPrioritizer::~PixelPrioritizer()
    {

    }

    void PixelPrioritizer::reset(
            const glm::ivec2& frameResolution,
            unsigned int blurWidth,
            double blurVariance)
    {
        unsigned int pixelCount =
                frameResolution.x *
                frameResolution.y;
        _tmpBuff.resize(pixelCount);

        computeGaussLine(_gauss, blurWidth, blurVariance);

        _frameMaxPriority = 1.0;
    }

    bool PixelPrioritizer::launchPrioritization(ConvergentFilm& film)
    {
//        using std::chrono::high_resolution_clock;
//        auto tStart = high_resolution_clock::now();

        glm::ivec2 frameResolution = film.frameResolution();
        std::vector<glm::dvec2>& refVarBuff = film._referenceFilm.varianceBuffer;
        std::vector<glm::dvec2>& rawVarBuff = film._varianceBuffer;
        std::vector<double>& prioBuff = film._priorityBuffer;

        size_t kernelWidth = _gauss.size();
        size_t halfWidth = kernelWidth / 2;
        for(int j=0; j<frameResolution.y; ++j)
        {
            unsigned int lineBaseIdx = j*frameResolution.x;

            for(int i=0; i<frameResolution.x; ++i)
            {
                unsigned int idx = lineBaseIdx + i;

                _tmpBuff[idx] = 0.0;
                double centerWeight = _gauss[halfWidth];
                for(int k=1; k <= halfWidth; ++k)
                {
                    if(i-k >= 0)
                    {
                        int index = lineBaseIdx+i-k;
                        const glm::dvec2& var = rawVarBuff[index];
                        const glm::dvec2& refVar = refVarBuff[index];
                        double compatibility = film.refShotCompatibility(index);
                        glm::dvec2 mixedVar = var + refVar * compatibility;

                        _tmpBuff[idx] += _gauss[halfWidth-k] * mixedVar.x / mixedVar.y;
                    }
                    else
                        centerWeight += _gauss[halfWidth-k];

                    if(i+k < frameResolution.x)
                    {
                        int index = lineBaseIdx+i+k;
                        const glm::dvec2& var = rawVarBuff[index];
                        const glm::dvec2& refVar = refVarBuff[index];
                        double compatibility = film.refShotCompatibility(index);
                        glm::dvec2 mixedVar = var + refVar * compatibility;

                        _tmpBuff[idx] += _gauss[halfWidth+k] * mixedVar.x / mixedVar.y;
                    }
                    else
                        centerWeight += _gauss[halfWidth+k];
                }

                const glm::dvec2& var = rawVarBuff[idx];
                const glm::dvec2& refVar = refVarBuff[idx];
                double compatibility = film.refShotCompatibility(idx);
                glm::dvec2 mixedVar = var + refVar * compatibility;

                _tmpBuff[idx] += centerWeight * mixedVar.x / mixedVar.y;
            }
        }

        for(int j=0; j<frameResolution.y; ++j)
        {
            unsigned int lineBaseIdx = j*frameResolution.x;

            for(int i=0; i<frameResolution.x; ++i)
            {
                unsigned int idx = lineBaseIdx + i;

                double bluredVar = 0.0;
                double centerWeight = _gauss[halfWidth];
                for(int k=1; k <= halfWidth; ++k)
                {
                    if(j-k >= 0)
                        bluredVar += _gauss[halfWidth-k] *
                            _tmpBuff[(j-k)*frameResolution.x + i];
                    else
                        centerWeight += _gauss[halfWidth-k];

                    if(j+k < frameResolution.y)
                        bluredVar += _gauss[halfWidth+k] *
                            _tmpBuff[(j+k)*frameResolution.x + i];
                    else
                        centerWeight += _gauss[halfWidth+k];
                }
                bluredVar += centerWeight * _tmpBuff[idx];


                double compatibility = film.refShotCompatibility(idx);
                glm::dvec4 mixedSample = film._sampleBuffer[idx] +
                    film._referenceFilm.sampleBuffer[idx] * compatibility;
                glm::dvec3 mixedCol = glm::dvec3(mixedSample) / mixedSample.w;
                mixedCol = glm::min(mixedCol, film._maxPixelIntensity);
                double mixedWeight = mixedSample.w;

                double divergence = film.toDivergence(
                    glm::dvec2(bluredVar, 1.0), mixedCol, mixedWeight);

                double weightPrio = mixedWeight*mixedWeight*mixedWeight;
                prioBuff[idx] = film._priorityScale *
                    (divergence + film._priorityWeightBias / weightPrio);
            }
        }

        _frameMaxPriority = 0.0;
        for(const std::shared_ptr<Tile>& tile : film._tiles)
        {
            double tileMaxPriority = 0.0;
            for(int j=tile->minCorner().y; j < tile->maxCorner().y; ++j)
            {
                int index = j * frameResolution.x + tile->minCorner().x;
                for(int i=tile->minCorner().x; i < tile->maxCorner().x; ++i, ++index)
                {
                    if(tileMaxPriority < prioBuff[index])
                        tileMaxPriority = prioBuff[index];
                }

                if(_frameMaxPriority < tileMaxPriority)
                    _frameMaxPriority = tileMaxPriority;
            }

            tile->setTilePriority(tileMaxPriority);
        }

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

    double PixelPrioritizer::maxFramePriority() const
    {
        return _frameMaxPriority;
    }

     void PixelPrioritizer::computeGaussLine(
            std::vector<double>& gauss,
            unsigned int width,
            double var)
     {
         float mass = 0.0;
         int halfSize = width / 2;
         gauss.resize(halfSize*2 + 1);

         for(int i=-halfSize; i<=halfSize; ++i)
         {
             int idx = i+halfSize;
             gauss[idx] = glm::exp(-(i*i) / (2.0*var));
             mass += gauss[idx];
         }

         for(int i=0; i<width; ++i)
            gauss[i] = gauss[i] / mass;
     }
}
