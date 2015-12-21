#include "ConvergentFilm.h"

#include <GLM/gtc/random.hpp>


namespace prop3
{
    ConvergentFilm::ConvergentFilm() :
        _varianceBuffer(1, 1.0),
        _divergenceBuffer(1, 0.0),
        _weightedColorBuffer(1, glm::dvec4(0)),
        _minThresholdFrameCount(3)
    {

    }

    ConvergentFilm::~ConvergentFilm()
    {

    }

    void ConvergentFilm::clear(const glm::dvec3& color, bool hardReset)
    {
        _newTileCompleted = false;
        _newFrameCompleted = false;

        _nextTileId = 0;
        _varianceThreshold = 0.0;


        size_t pixelCount = _frameResolution.x * _frameResolution.y;

        _colorBuffer.clear();
        _colorBuffer.resize(pixelCount, color);

        _varianceBuffer.clear();
        _varianceBuffer.resize(pixelCount, 1.0);

        _divergenceBuffer.clear();
        _divergenceBuffer.resize(pixelCount, 0.0);

        glm::dvec4 sample(color, 0.0);
        _weightedColorBuffer.clear();
        _weightedColorBuffer.resize(pixelCount, sample);

        for(const auto& tile : _tiles)
        {
            tile->setTileVariance(1.0);
        }
    }

    double ConvergentFilm::compileDivergence() const
    {
        double meanDivergenceSqr = 0.0;
        size_t buffSize = _divergenceBuffer.size();
        for(size_t i=0; i < buffSize; ++i)
        {
            meanDivergenceSqr += _divergenceBuffer[i];
        }

        meanDivergenceSqr /= double(buffSize);
        double stdDev = glm::sqrt(meanDivergenceSqr);

        return stdDev;
    }

    void ConvergentFilm::tileCompleted(Tile& tile)
    {
        std::lock_guard<std::mutex> lk(_tilesMutex);

        _newTileCompleted = true;

        double maxVariance = 0.0;
        if(_framePassCount >= _minThresholdFrameCount)
        {
            for(int j=tile.minCorner().y; j < tile.maxCorner().y; ++j)
            {
                for(int i=tile.minCorner().x; i < tile.maxCorner().x; ++i)
                {
                    maxVariance = glm::max(maxVariance, pixelVariance(i, j));
                }
            }
        }
        else maxVariance = 1.0;
        tile.setTileVariance(maxVariance);
    }

    void ConvergentFilm::endTileReached()
    {
        _nextTileId = 0;
        _newFrameCompleted = true;

        _cvMutex.lock();
        _newFrameCompleted = true;
        _cvMutex.unlock();
        _cv.notify_all();

        if(_framePassCount >= _minThresholdFrameCount)
        {
            int index = 0;
            double meanVariance = 0.0;
            for(int j=0; j < frameHeight(); ++j)
            {
                for(int i=0; i < frameWidth(); ++i)
                {
                    meanVariance += glm::min(_varianceBuffer[index], 3.0);
                    ++index;
                }
            }
            meanVariance /= _varianceBuffer.size();

            _varianceThreshold = glm::linearRand(0.0, meanVariance);
        }
    }

    glm::dvec4 ConvergentFilm::sample(int index) const
    {
        return _weightedColorBuffer[index];
    }

    double ConvergentFilm::pixelVariance(int index) const
    {
        if(_framePassCount >= _minThresholdFrameCount)
            return _varianceBuffer[index];
        else
            return 1.0;
    }

    void ConvergentFilm::setColor(int index, const glm::dvec3& color)
    {
        _colorBuffer[index] = color;
    }

    void ConvergentFilm::addSample(int index, const glm::dvec4& sample)
    {
        glm::dvec4 oldSample = _weightedColorBuffer[index];
        glm::dvec4 newSample = oldSample + sample;
        _weightedColorBuffer[index] = newSample;

        glm::dvec3 newColor = glm::dvec3(newSample) / newSample.w;

        if(oldSample.w != 0.0)
        {
            glm::dvec3 oldColor = glm::dvec3(oldSample) / oldSample.w;
            glm::dvec3 sampColor = glm::dvec3(sample) / sample.w;

            glm::dvec3 delta = oldColor - newColor;
            _divergenceBuffer[index] = glm::dot(delta, delta);

            glm::dvec3 dColor = sampColor - oldColor;
            double dMean = glm::dot(dColor, dColor);
            double oldVar = _varianceBuffer[index];
            _varianceBuffer[index] = glm::mix(_varianceBuffer[index], dMean,
                (oldVar==1.0 ? 1.0 : (sample.w / newSample.w)));
        }

        _colorBuffer[index] = newColor;
    }
}
