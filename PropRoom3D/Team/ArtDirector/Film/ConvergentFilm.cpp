#include "ConvergentFilm.h"

#include <GLM/gtc/random.hpp>


namespace prop3
{
    ConvergentFilm::ConvergentFilm() :
        _varianceBuffer(1, 0.0),
        _divergenceBuffer(1, 0.0),
        _weightedColorBuffer(1, glm::dvec4(0)),
        _minThresholdFrameCount(7),
        _forceFullFrameCycleCount(8),
        _maxConsideredPriority(5.0),
        _maxPixelIntensity(2.5)
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
        _framePassCount = 0;
        _priorityThreshold = 0.0;


        size_t pixelCount = _frameResolution.x * _frameResolution.y;

        _colorBuffer.clear();
        _colorBuffer.resize(pixelCount, color);

        _varianceBuffer.clear();
        _varianceBuffer.resize(pixelCount, 0.0);

        _divergenceBuffer.clear();
        _divergenceBuffer.resize(pixelCount, 0.0);

        glm::dvec4 sample(color, 0.0);
        _weightedColorBuffer.clear();
        _weightedColorBuffer.resize(pixelCount, sample);

        for(const auto& tile : _tiles)
        {
            tile->setTilePriority(1.0);
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

        if(_framePassCount >= _minThresholdFrameCount)
        {
            double maxPriority = 0.0;
            for(int j=tile.minCorner().y; j < tile.maxCorner().y; ++j)
                for(int i=tile.minCorner().x; i < tile.maxCorner().x; ++i)
                    maxPriority = glm::max(maxPriority, pixelPriority(i, j));

            tile.setTilePriority(maxPriority);
        }
    }

    void ConvergentFilm::endTileReached()
    {
        _nextTileId = 0;
        _newFrameCompleted = true;

        _cvMutex.lock();
        _newFrameCompleted = true;
        _cvMutex.unlock();
        _cv.notify_all();

        if(_framePassCount >= _minThresholdFrameCount &&
           _framePassCount % _forceFullFrameCycleCount != 0)
        {
            int index = 0;
            double meanPriority = 0.0;
            for(int j=0; j < frameHeight(); ++j)
                for(int i=0; i < frameWidth(); ++i, ++index)
                    meanPriority += pixelPriority(index);

            meanPriority /= _varianceBuffer.size();
            double baseRand = glm::linearRand(0.0, 1.0);

            _priorityThreshold = glm::sqrt(baseRand) * meanPriority;
        }
        else
        {
            _priorityThreshold = 0.0;
        }
    }

    glm::dvec4 ConvergentFilm::sample(int index) const
    {
        return _weightedColorBuffer[index];
    }

    double ConvergentFilm::pixelPriority(int index) const
    {
        double weight =_weightedColorBuffer[index].w;
        if(_framePassCount >= _minThresholdFrameCount && weight > 0.0)
        {
            return glm::min(_varianceBuffer[index] + 1.0 / (weight* weight),
                            _maxConsideredPriority);
        }

        return _maxConsideredPriority;
    }

    void ConvergentFilm::setColor(int index, const glm::dvec3& color)
    {
        _colorBuffer[index] = color;
    }

    void ConvergentFilm::addSample(int index, const glm::dvec4& sample)
    {
        // Power heuristic weight optimization
        glm::dvec4 trueSample = sample * sample.w;

        glm::dvec4 oldSample = _weightedColorBuffer[index];
        glm::dvec4 newSample = oldSample + trueSample;
        _weightedColorBuffer[index] = newSample;

        glm::dvec3 newColor = glm::dvec3(newSample) / newSample.w;

        if(oldSample.w != 0.0)
        {
            glm::dvec3 oldColor = glm::min(_maxPixelIntensity,
                glm::dvec3(oldSample) / oldSample.w);
            glm::dvec3 sampColor = glm::min(_maxPixelIntensity,
                glm::dvec3(trueSample) / trueSample.w);

            glm::dvec3 delta = oldColor - newColor;
            _divergenceBuffer[index] = glm::dot(delta, delta);


            glm::dvec3 dColor = sampColor - oldColor;
            double dMean = glm::dot(dColor, dColor);
            double dWeight = glm::min(trueSample.w, oldSample.w);
            double dBase = glm::max(trueSample.w, oldSample.w);

            double oldVar = _varianceBuffer[index];
            _varianceBuffer[index] = glm::mix(oldVar, dMean, dWeight / dBase);
        }

/*
        double priority = _maxConsideredPriority;
        double weight = _weightedColorBuffer[index].w;
        if(weight > 0.0)
            priority = glm::min(_varianceBuffer[index] + 1.0 / (weight*weight),
                                _maxConsideredPriority);

        _colorBuffer[index] = glm::dvec3(priority);
//*/
        _colorBuffer[index] = newColor;
    }
}
