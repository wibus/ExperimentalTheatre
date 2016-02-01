#include "ConvergentFilm.h"


namespace prop3
{
    ConvergentFilm::ConvergentFilm() :
        _varianceBuffer(1, 0.0),
        _divergenceBuffer(1, 1.0),
        _priorityBuffer(1, 1.0),
        _weightedColorBuffer(1, glm::dvec4(0)),
        _minThresholdFrameCount(8),
        _maxPixelIntensity(2.0)
    {

    }

    ConvergentFilm::~ConvergentFilm()
    {

    }

    const std::vector<glm::vec3>& ConvergentFilm::colorBuffer(ColorOutput colorOutput)
    {
        if(colorOutput != _colorOutput)
        {
            _colorOutput = colorOutput;

            int pixelCount = _frameResolution.x * _frameResolution.y;

            switch(colorOutput)
            {
            case ColorOutput::ALBEDO :
                for(int i=0; i < pixelCount; ++i)
                    _colorBuffer[i] = sampleToColor(_weightedColorBuffer[i]);
                break;

            case ColorOutput::DIVERGENCE :
                for(int i=0; i < pixelCount; ++i)
                    _colorBuffer[i] = divergenceToColor(_divergenceBuffer[i]);
                break;

            case ColorOutput::VARIANCE :
                for(int i=0; i < pixelCount; ++i)
                    _colorBuffer[i] = varianceToColor(_varianceBuffer[i]);
                break;

            case ColorOutput::PRIORITY :
                for(int i=0; i < pixelCount; ++i)
                    _colorBuffer[i] = priorityToColor(_priorityBuffer[i]);
                break;
            }
        }

        return _colorBuffer;
    }

    void ConvergentFilm::clear(const glm::dvec3& color, bool hardReset)
    {
        size_t pixelCount = _frameResolution.x * _frameResolution.y;

        _newTileCompleted = false;
        _newFrameCompleted = false;

        _nextTileId = 0;
        _framePassCount = 0;
        _priorityThreshold = 0.0;


        _colorBuffer.clear();
        _colorBuffer.resize(pixelCount, color);

        _varianceBuffer.clear();
        _varianceBuffer.resize(pixelCount, 0.0);

        _divergenceBuffer.clear();
        _divergenceBuffer.resize(pixelCount, 1.0);

        _priorityBuffer.clear();
        _priorityBuffer.resize(pixelCount, 1.0);

        glm::dvec4 sample(color, 0.0);
        _weightedColorBuffer.clear();
        _weightedColorBuffer.resize(pixelCount, sample);

        for(const auto& tile : _tiles)
        {
            glm::ivec2 tileResolution = tile->maxCorner() - tile->minCorner();
            int tilePixCount = tileResolution.x * tileResolution.y;

            tile->setTilePriority(1.0);
            tile->setPrioritySum(tilePixCount);
            tile->setDivergenceSum(tilePixCount);
        }
    }

    double ConvergentFilm::compileDivergence() const
    {
        double divSum = 0.0;
        double tileCount = _tiles.size();
        for(size_t i=0; i < tileCount; ++i)
            divSum += _tiles[i]->divergenceSum();

        double divCount = _divergenceBuffer.size();
        return glm::sqrt(divSum / divCount);
    }

    void ConvergentFilm::tileCompleted(Tile& tile)
    {
        _newTileCompleted = true;

        double prioritySum = 0.0;
        double maxPriority = 0.0;
        double divergenceSum = 0.0;
        for(int j=tile.minCorner().y; j < tile.maxCorner().y; ++j)
        {
            int index = j * _frameResolution.x + tile.minCorner().x;
            for(int i=tile.minCorner().x; i < tile.maxCorner().x; ++i, ++index)
            {
                prioritySum += _priorityBuffer[index] * _priorityBuffer[index];
                maxPriority = glm::max(maxPriority, _priorityBuffer[index]);
                divergenceSum += _divergenceBuffer[index];
            }
        }

        tile.setPrioritySum(prioritySum);
        tile.setTilePriority(maxPriority);
        tile.setDivergenceSum(divergenceSum);
    }

    void ConvergentFilm::endTileReached()
    {
        _cvMutex.lock();
        _nextTileId = 0;
        _newFrameCompleted = true;
        _cvMutex.unlock();
        _cv.notify_all();

        if(_framePassCount >= _minThresholdFrameCount)
        {
            double prioritySum = 0.0;
            double tileCount = _tiles.size();
            for(size_t i=0; i < tileCount; ++i)
                prioritySum += _tiles[i]->prioritySum();

            double baseRand = _linearRand.gen1(0.0, 1.0);
            double meanPriority = prioritySum / _priorityBuffer.size();
            _priorityThreshold = 0.75 * glm::sqrt(baseRand * meanPriority);
        }
        else
        {
            _priorityThreshold = 0.0;
        }
    }

    glm::dvec4 ConvergentFilm::pixelSample(int index) const
    {
        return _weightedColorBuffer[index];
    }

    double ConvergentFilm::pixelDivergence(int index) const
    {
        return _divergenceBuffer[index];
    }

    double ConvergentFilm::pixelPriority(int index) const
    {
        return _priorityBuffer[index];
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

        double newWeight = newSample.w;
        glm::dvec3 newColor = glm::dvec3(newSample) / newWeight;

        if(_colorOutput == ColorOutput::ALBEDO)
            _colorBuffer[index] = newColor;

        if(oldSample.w != 0.0)
        {
            glm::dvec3 oldColor = glm::min(_maxPixelIntensity,
                glm::dvec3(oldSample) / oldSample.w);
            glm::dvec3 sampColor = glm::min(_maxPixelIntensity,
                glm::dvec3(trueSample) / trueSample.w);

            glm::dvec3 delta = oldColor - glm::min(_maxPixelIntensity, newColor);
            _divergenceBuffer[index] = glm::dot(delta, delta);

            if(_colorOutput == ColorOutput::DIVERGENCE)
                _colorBuffer[index] = divergenceToColor(_divergenceBuffer[index]);


            glm::dvec3 dColor = sampColor - oldColor;
            double dMean = glm::dot(dColor, dColor);
            double dWeight = glm::min(trueSample.w, oldSample.w);
            double dBase = glm::max(trueSample.w, oldSample.w);

            double oldVar = _varianceBuffer[index];
            double newVar = glm::mix(oldVar, dMean, dWeight / dBase);
            _varianceBuffer[index] = newVar;

            if(_colorOutput == ColorOutput::VARIANCE)
                _colorBuffer[index] = varianceToColor(_varianceBuffer[index]);


            double scale = glm::length(newColor) + 1e-3;
            _priorityBuffer[index] = (newVar / scale  + 0.1/newWeight) / newWeight;

            if(_colorOutput == ColorOutput::PRIORITY)
                _colorBuffer[index] = priorityToColor(_priorityBuffer[index]);
        }
    }


    glm::vec3 ConvergentFilm::sampleToColor(const glm::dvec4& sample) const
    {
        if(sample.w > 0.0)
            return glm::vec3(sample) / float(sample.w);
        else
            return glm::vec3(0.0);
    }

    glm::vec3 ConvergentFilm::divergenceToColor(double divergence) const
    {
        return glm::vec3(divergence * 200.0);
    }

    glm::vec3 ConvergentFilm::varianceToColor(double variance) const
    {
        return glm::vec3(variance * 10.0);
    }

    glm::vec3 ConvergentFilm::priorityToColor(double priority) const
    {
        return glm::vec3(priority * 50.0);
    }
}
