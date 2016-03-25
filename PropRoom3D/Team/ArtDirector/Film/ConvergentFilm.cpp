#include "ConvergentFilm.h"

#include <algorithm>


namespace prop3
{
    ConvergentFilm::ConvergentFilm() :
        _weightedVarianceBuffer(1, glm::dvec2(0)),
        _divergenceBuffer(1, 1.0),
        _priorityBuffer(1, 1.0),
        _weightedColorBuffer(1, glm::dvec4(0)),
        _varianceWeightThreshold(4.0),
        _priorityWeightThreshold(7.0),
        _maxPixelIntensity(2.0)
    {
        _priorityWeightBias =
            _priorityWeightThreshold *
            _priorityWeightThreshold;

        _priorityScale =
            (_priorityWeightThreshold *
             _priorityWeightThreshold *
             _priorityWeightThreshold ) /
                _priorityWeightBias ;
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

            case ColorOutput::WEIGHT :
                for(int i=0; i < pixelCount; ++i)
                    _colorBuffer[i] = weightToColor(_weightedColorBuffer[i]);
                break;

            case ColorOutput::DIVERGENCE :
                for(int i=0; i < pixelCount; ++i)
                    _colorBuffer[i] = divergenceToColor(_divergenceBuffer[i]);
                break;

            case ColorOutput::VARIANCE :
                for(int i=0; i < pixelCount; ++i)
                    _colorBuffer[i] = varianceToColor(_weightedVarianceBuffer[i]);
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
        _priorityThreshold = 1.0;
        _maxPixelWeight = 1.0;


        _colorBuffer.clear();
        _colorBuffer.resize(pixelCount, color);

        _weightedVarianceBuffer.clear();
        _weightedVarianceBuffer.resize(pixelCount, glm::dvec2(0));

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
            tile->setDivergenceSum(tilePixCount);
        }
    }

    double ConvergentFilm::compileDivergence() const
    {
        double tileCount = _tiles.size();
        std::vector<double> tileVal(tileCount);
        for(size_t i=0; i < tileCount; ++i)
            tileVal[i] = _tiles[i]->divergenceSum()
                           / _tiles[i]->pixelCount();

        std::sort(tileVal.begin(), tileVal.end(), std::greater<double>());

        double meanSum = 0;
        const int MEAN_TILE_COUNT = 16;
        for(int i=0; i < MEAN_TILE_COUNT; ++i)
            meanSum += glm::sqrt(tileVal[i]);

        return meanSum / MEAN_TILE_COUNT;
    }

    void ConvergentFilm::tileCompleted(Tile& tile)
    {
        _newTileCompleted = true;

        if(_framePassCount >= _priorityWeightThreshold)
        {
            double maxPriority = 0.0;
            double divergenceSum = 0.0;
            for(int j=tile.minCorner().y; j < tile.maxCorner().y; ++j)
            {
                int index = j * _frameResolution.x + tile.minCorner().x;
                for(int i=tile.minCorner().x; i < tile.maxCorner().x; ++i, ++index)
                {
                    double div = _divergenceBuffer[index];
                    divergenceSum += div * div;

                    if(maxPriority < _priorityBuffer[index])
                        maxPriority = _priorityBuffer[index];

                    double currWeight = _weightedColorBuffer[index].w;
                    if(_maxPixelWeight < currWeight)
                        _maxPixelWeight = currWeight;
                }
            }

            tile.setTilePriority(maxPriority);
            tile.setDivergenceSum(divergenceSum);
        }
    }

    void ConvergentFilm::endTileReached()
    {
        _cvMutex.lock();
        _nextTileId = 0;
        _newFrameCompleted = true;
        _cvMutex.unlock();
        _cv.notify_all();

        if(_framePassCount > _priorityWeightThreshold)
        {
            double topPriority = 0.0;
            double tileCount = _tiles.size();
            for(size_t i=0; i < tileCount; ++i)
            {
                double tilePriority = _tiles[i]->tilePriority();
                if(topPriority < tilePriority)
                    topPriority = tilePriority;
            }

            topPriority = glm::min(topPriority, 1.0);
            double baseRand = _linearRand.gen1(0.0, 1.0);
            _priorityThreshold = baseRand * topPriority * 0.85;
        }
        else
        {
            _priorityThreshold = 1.0;
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
        glm::dvec4 oldSample = _weightedColorBuffer[index];
        glm::dvec4 newSample = oldSample + sample;
        _weightedColorBuffer[index] = newSample;

        glm::dvec3 newColor = sampleToColor(newSample);
        if(_colorOutput == ColorOutput::ALBEDO)
            _colorBuffer[index] = newColor;

        if(_colorOutput == ColorOutput::WEIGHT)
            _colorBuffer[index] = weightToColor(newSample);

        double oldWeight = oldSample.w;
        if(oldWeight > _varianceWeightThreshold)
        {
            glm::dvec3 oldColor = glm::min(_maxPixelIntensity,
                glm::dvec3(oldSample) / oldSample.w);
            glm::dvec3 sampColor = glm::min(_maxPixelIntensity,
                glm::dvec3(sample) / sample.w);

            double sampWeight = sample.w;
            glm::dvec3 dColor = sampColor - oldColor;
            double dMean = glm::length(dColor) * sampWeight;

            _weightedVarianceBuffer[index] += glm::dvec2(dMean * sampWeight, sampWeight);
            glm::dvec2 newWeightedVar = _weightedVarianceBuffer[index];

            if(_colorOutput == ColorOutput::VARIANCE)
                _colorBuffer[index] = varianceToColor(newWeightedVar);


            double newWeight = newSample.w;
            if(newWeight > _priorityWeightThreshold)
            {
                const double WEIGHT_OFFSET = 0.25;
                double scale = glm::length(newColor) + WEIGHT_OFFSET;
                double newVar = newWeightedVar.x / newWeightedVar.y;
                double newDiv = newVar / (scale * newWeight);
                _divergenceBuffer[index] = newDiv;

                if(_colorOutput == ColorOutput::DIVERGENCE)
                    _colorBuffer[index] = divergenceToColor(newDiv);


                double weightPrio = newWeight*newWeight*newWeight;
                double newPrio = _priorityScale *
                    (newDiv + _priorityWeightBias / weightPrio);
                _priorityBuffer[index] = newPrio;

                if(_colorOutput == ColorOutput::PRIORITY)
                    _colorBuffer[index] = priorityToColor(newPrio);
            }
        }
    }


    glm::vec3 ConvergentFilm::sampleToColor(const glm::dvec4& sample) const
    {
        if(sample.w > 0.0)
            return glm::vec3(sample) / float(sample.w);
        else
            return glm::vec3(0.0);
    }

    glm::vec3 ConvergentFilm::weightToColor(const glm::dvec4& sample) const
    {
        if(sample.w > _priorityWeightThreshold)
        {
            double maxPrio =_maxPixelWeight - _priorityWeightThreshold;
            double currWeight = sample.w - _priorityWeightThreshold;
            return glm::vec3(currWeight / maxPrio);
        }
        else
            return glm::vec3(0.0);
    }

    glm::vec3 ConvergentFilm::divergenceToColor(double divergence) const
    {
        return glm::vec3(divergence * 10.0);
    }

    glm::vec3 ConvergentFilm::varianceToColor(const glm::dvec2& variance) const
    {
        if(variance.y > 0.0)
            return glm::vec3((variance.x / variance.y) * 2.0);
        else
            return glm::dvec3(1.0);
    }

    glm::vec3 ConvergentFilm::priorityToColor(double priority) const
    {
        return glm::vec3(priority);
    }
}
