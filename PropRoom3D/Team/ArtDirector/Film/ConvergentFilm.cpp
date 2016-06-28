#include "ConvergentFilm.h"

#include <fstream>
#include <algorithm>
#include <numeric>

#include "PixelPrioritizer.h"


namespace prop3
{
    const double UINT_MAX_DOUBLE = std::numeric_limits<unsigned short>::max();
    const double ConvergentFilm::RawPixel::COLOR_SCALING = 1 / 8.0;
    const double ConvergentFilm::RawPixel::VARIANCE_SCALING = 1 / 16.0;
    const double ConvergentFilm::RawPixel::COLOR_DECOMPRESSION = 8.0 / UINT_MAX_DOUBLE;
    const double ConvergentFilm::RawPixel::VARIANCE_DECOMPRESSION = 16.0 / UINT_MAX_DOUBLE;

    ConvergentFilm::RawPixel::RawPixel() :
        weight(0.0), v(0), r(0), g(0), b(0)
    {

    }

    ConvergentFilm::RawPixel::RawPixel(
            const glm::dvec4& sample,
            const glm::dvec2& variance)
    {
        var = variance.x;
        weight = sample.w;
        double weightInv = 1 / weight;
        vw = glm::min((weight - variance.y) * VARIANCE_SCALING, 1.0) * std::numeric_limits<unsigned short>::max();
        r = glm::min(glm::sqrt(sample.r * weightInv) * COLOR_SCALING, 1.0) * std::numeric_limits<unsigned short>::max();
        g = glm::min(glm::sqrt(sample.g * weightInv) * COLOR_SCALING, 1.0) * std::numeric_limits<unsigned short>::max();
        b = glm::min(glm::sqrt(sample.b * weightInv) * COLOR_SCALING, 1.0) * std::numeric_limits<unsigned short>::max();
    }

    void ConvergentFilm::RawPixel::toRaw(
            glm::dvec4& sample,
            glm::dvec2& variance) const
    {
        variance.x = var;
        sample.w = weight;
        variance.y = weight - vw * VARIANCE_DECOMPRESSION;
        sample.r = r * COLOR_DECOMPRESSION;
        sample.r = weight * (sample.r * sample.r);
        sample.g = g * COLOR_DECOMPRESSION;
        sample.g = weight * (sample.g * sample.g);
        sample.b = b * COLOR_DECOMPRESSION;
        sample.b = weight * (sample.b * sample.b);
    }


    ConvergentFilm::ConvergentFilm() :
        _sampleBuffer(1, glm::dvec4(0)),
        _varianceBuffer(1, glm::dvec2(0)),
        _refMeanDistBuffer(1, glm::dvec2(0)),
        _compatibilityBuffer(1.0, 0.0),
        _divergenceBuffer(1, 1.0),
        _priorityBuffer(1, 1.0),
        _condifdenceRange(0.75),
        _varianceWeightThreshold(4.0),
        _priorityWeightThreshold(7.0),
        _maxPixelIntensity(1.5),
        _prioritizer(new PixelPrioritizer())
    {
        _priorityWeightBias = 0.25 *
            _priorityWeightThreshold *
            _priorityWeightThreshold;

        _priorityScale =
            (_priorityWeightThreshold *
             _priorityWeightThreshold *
             _priorityWeightThreshold ) /
                _priorityWeightBias;
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
                    _colorBuffer[i] = sampleToColor(_sampleBuffer[i] +
                        _referenceFilm.sampleBuffer[i] * refCompatibility(i));
                break;

            case ColorOutput::WEIGHT :
                for(int i=0; i < pixelCount; ++i)
                    _colorBuffer[i] = weightToColor(_sampleBuffer[i]);
                break;

            case ColorOutput::DIVERGENCE :
                for(int i=0; i < pixelCount; ++i)
                    _colorBuffer[i] = divergenceToColor(_divergenceBuffer[i]);
                break;

            case ColorOutput::VARIANCE :
                for(int i=0; i < pixelCount; ++i)
                    _colorBuffer[i] = varianceToColor(_varianceBuffer[i] +
                        _referenceFilm.varianceBuffer[i] * refCompatibility(i));
                break;

            case ColorOutput::COMPATIBILITY :
                for(int i=0; i < pixelCount; ++i)
                    _colorBuffer[i] = compatibilityToColor(refCompatibility(i));
                break;

            case ColorOutput::PRIORITY :
                _prioritizer->displayPrioritization(*this);
                break;

            case ColorOutput::REFERENCE :
                for(int i=0; i < pixelCount; ++i)
                    _colorBuffer[i] = sampleToColor(_referenceFilm.sampleBuffer[i]);
                break;
            }
        }

        return _colorBuffer;
    }

    void ConvergentFilm::resetFilmState()
    {
        size_t pixelCount = _frameResolution.x * _frameResolution.y;

        _tileCompletedCount = 0;
        _newTileCompleted = false;
        _newFrameCompleted = false;

        _nextTileId = 0;
        _framePassCount = 0;
        _priorityThreshold = 1.0;
        _sampleMultiplicity = 8.0;

        while(!_tileMsgs.empty())
            _tileMsgs.pop();

        _prioritizer->reset(_frameResolution);

        for(const auto& tile : _tiles)
        {
            tile->setTilePriority(1.0);
            tile->setDivergence(1.0);
        }
    }

    void ConvergentFilm::clearBuffers(const glm::dvec3& color)
    {
        size_t pixelCount = _frameResolution.x * _frameResolution.y;

        _sampleBuffer.clear();
        _sampleBuffer.resize(pixelCount, glm::dvec4(0));

        _varianceBuffer.clear();
        _varianceBuffer.resize(pixelCount, glm::dvec2(0));

        _refMeanDistBuffer.clear();
        _refMeanDistBuffer.resize(pixelCount, glm::dvec2(0));

        _compatibilityBuffer.clear();
        _compatibilityBuffer.resize(pixelCount, 0.0);

        _divergenceBuffer.clear();
        _divergenceBuffer.resize(pixelCount, 1.0);

        _priorityBuffer.clear();
        _priorityBuffer.resize(pixelCount, 1.0);

        _colorBuffer.clear();
        _colorBuffer.resize(pixelCount, color);

        if(pixelCount != _referenceFilm.sampleBuffer.size())
        {
            clearReferenceShot();
        }
    }

    void ConvergentFilm::backupAsReferenceShot()
    {
        size_t pixelCount = _sampleBuffer.size();
        _referenceFilm.sampleBuffer.resize(pixelCount);
        _referenceFilm.varianceBuffer.resize(pixelCount);
        for(size_t i=0; i < pixelCount; ++i)
        {
            const glm::dvec4& curSamp = _sampleBuffer[i];
            const glm::dvec2& curVar = _varianceBuffer[i];
            const glm::dvec4& refSamp = _referenceFilm.sampleBuffer[i];
            const glm::dvec2& refVar = _referenceFilm.varianceBuffer[i];

            double compatibility = refCompatibility(i);

            _referenceFilm.sampleBuffer[i] = curSamp + refSamp * compatibility;
            _referenceFilm.varianceBuffer[i] = curVar + refVar * compatibility;
        }
    }

    bool ConvergentFilm::saveReferenceShot(const std::string& name) const
    {
        return saveContent(name,
            _referenceFilm.sampleBuffer,
            _referenceFilm.varianceBuffer);
    }

    bool ConvergentFilm::loadReferenceShot(const std::string& name)
    {
        size_t pixelCount = _frameResolution.x * _frameResolution.y;
        if(pixelCount != _referenceFilm.sampleBuffer.size())
        {
            clearReferenceShot();
        }

        if(loadContent(name,
            _referenceFilm.sampleBuffer,
            _referenceFilm.varianceBuffer))
        {
            if(_colorOutput == ColorOutput::REFERENCE)
            {
                for(int i=0; i < pixelCount; ++i)
                    _colorBuffer[i] = sampleToColor(_referenceFilm.sampleBuffer[i]);
            }

            return true;
        }

        return false;
    }

    bool ConvergentFilm::clearReferenceShot()
    {
        size_t pixelCount = _sampleBuffer.size();

        _referenceFilm.sampleBuffer.clear();
        _referenceFilm.sampleBuffer.resize(pixelCount, glm::dvec4(0));

        _referenceFilm.varianceBuffer.clear();
        _referenceFilm.varianceBuffer.resize(pixelCount, glm::dvec2(0));

        if(_colorOutput == ColorOutput::REFERENCE)
        {
            for(int i=0; i < pixelCount; ++i)
                _colorBuffer[i] = sampleToColor(_referenceFilm.sampleBuffer[i]);
        }
    }

    bool ConvergentFilm::saveRawFilm(const std::string& name) const
    {
        return saveContent(name,
            _sampleBuffer,
            _varianceBuffer);
    }

    bool ConvergentFilm::loadRawFilm(const std::string& name)
    {
        clearBuffers(glm::dvec3());
        return loadContent(name,
            _sampleBuffer,
            _varianceBuffer);
    }

    bool ConvergentFilm::saveContent(
            const std::string& name,
            const std::vector<glm::dvec4>& samples,
            const std::vector<glm::dvec2>& variances) const
    {
        std::ofstream film(name, std::ios_base::trunc | std::ios_base::binary);

        if(film.is_open())
        {
            size_t pixelCount = samples.size();
            _rawPixelPool.resize(pixelCount);
            for(size_t p=0; p < pixelCount; ++p)
                _rawPixelPool[p] = RawPixel(samples[p], variances[p]);

            film.write((char*)_rawPixelPool.data(),
                       sizeof(_rawPixelPool.front()) *
                       _rawPixelPool.size());
            film.close();
        }
        else
        {
            return false;
        }
    }

    bool ConvergentFilm::loadContent(
            const std::string& name,
            std::vector<glm::dvec4>& samples,
            std::vector<glm::dvec2>& variances)
    {
        std::ifstream film(name, std::ios_base::binary);

        if(film.is_open())
        {
            size_t pixelCount = samples.size();
            _rawPixelPool.resize(pixelCount);
            film.read((char*)_rawPixelPool.data(),
                      sizeof(_rawPixelPool.front()) *
                      _rawPixelPool.size());
            film.close();

            for(size_t p=0; p < pixelCount; ++p)
                _rawPixelPool[p].toRaw(samples[p], variances[p]);


            // Compute film divergence &
            // output new film in specified color buffer
            for(int p=0; p < pixelCount; ++p)
                addSample(p, glm::dvec4(0.0));
        }
        else
        {
            return false;
        }
    }

    double ConvergentFilm::compileDivergence() const
    {
        double tileCount = _tiles.size();
        std::vector<double> tileVal(tileCount);
        for(size_t i=0; i < tileCount; ++i)
            tileVal[i] = _tiles[i]->divergence();

        const int MEAN_TILE_COUNT = 16;
        std::nth_element(tileVal.begin(), tileVal.begin() + MEAN_TILE_COUNT,
                         tileVal.end(), std::greater<double>());

        double meanSum = 0;
        for(int i=0; i < MEAN_TILE_COUNT; ++i)
            meanSum += tileVal[i];

        return meanSum / MEAN_TILE_COUNT;
    }

    void ConvergentFilm::tileCompleted(Tile& tile)
    {
        _tilesMutex.lock();
        ++_tileCompletedCount;
        if(_tileCompletedCount == tileCount())
        {
            // Remove weight multiplicity after first complet frame
            _sampleMultiplicity = 1.0;

            _cvMutex.lock();
            ++_framePassCount;
            _newFrameCompleted = true;
            _cvMutex.unlock();
            _cv.notify_all();
        }
        _tilesMutex.unlock();


        double divergenceSum = 0.0;
        for(int j=tile.minCorner().y; j < tile.maxCorner().y; ++j)
        {
            int index = j * _frameResolution.x + tile.minCorner().x;

            for(int i=tile.minCorner().x; i < tile.maxCorner().x; ++i, ++index)
            {
                divergenceSum += _divergenceBuffer[index];
            }
        }

        tile.setDivergence(divergenceSum / tile.pixelCount());

        _newTileCompleted = true;
    }

    void ConvergentFilm::rewindTiles()
    {
        _nextTileId = 0;
    }

    bool ConvergentFilm::incomingTileAvailable() const
    {
        return !_tileMsgs.empty();
    }

    std::shared_ptr<TileMessage> ConvergentFilm::nextIncomingTile()
    {
        std::shared_ptr<TileMessage> msg;

        _tileMsgMutex.lock();
        if(!_tileMsgs.empty())
        {
            msg = _tileMsgs.front();
            _tileMsgs.pop();
        }
        _tileMsgMutex.unlock();

        return msg;
    }

    void ConvergentFilm::addIncomingTile(const std::shared_ptr<TileMessage>& msg)
    {
        _tileMsgMutex.lock();
        _tileMsgs.push(msg);
        _tileMsgMutex.unlock();
    }

    void ConvergentFilm::endTileReached()
    {
        if(_framePassCount > 0)
        {
            // Reprioritize frame's pixels
            _prioritizer->launchPrioritization(*this);
            _priorityThreshold = _prioritizer->priorityThreshold();

            if(_colorOutput == Film::ColorOutput::PRIORITY)
            {
                _prioritizer->displayPrioritization(*this);
            }

            _cvMutex.lock();
            _nextTileId = 0;
            ++_framePassCount;
            _newFrameCompleted = true;
            _cvMutex.unlock();
            _cv.notify_all();
        }
    }

    glm::dvec4 ConvergentFilm::pixelSample(int index) const
    {
        return _sampleBuffer[index];
    }

    double ConvergentFilm::pixelDivergence(int index) const
    {
        return _divergenceBuffer[index];
    }

    double ConvergentFilm::pixelPriority(int index) const
    {
        return _priorityBuffer[index];
    }
    void ConvergentFilm::addSample(int index, const glm::dvec4& sample)
    {
        glm::dvec4 oldSample = _sampleBuffer[index];
        glm::dvec4 newSample = oldSample + sample;
        _sampleBuffer[index] = newSample;

        double oldWeight = oldSample.w;
        if(oldWeight > _varianceWeightThreshold)
        {
            double sampWeight = sample.w;
            glm::dvec3 sampColor = glm::dvec3(sample);
            if(sampWeight > 0.0)
            {
                sampColor = glm::min(
                    _maxPixelIntensity,
                    sampColor / sampWeight);
            }

            glm::dvec3 oldColor = glm::min(_maxPixelIntensity,
                glm::dvec3(oldSample) / oldSample.w);
            glm::dvec3 dColor = sampColor - oldColor;
            double dMean = glm::dot(dColor, dColor) * sampWeight;

            _varianceBuffer[index] += glm::dvec2(dMean * sampWeight, sampWeight);
            glm::dvec2 newWeightedVar = _varianceBuffer[index];


            double compatibility = 0.0;
            if(_referenceFilm.sampleBuffer[index].w > 0.0)
            {
                glm::dvec3 refCol = glm::dvec3(
                        _referenceFilm.sampleBuffer[index]) /
                            _referenceFilm.sampleBuffer[index].w;
                glm::dvec3 dRefCol = sampColor - refCol;
                double dRef = glm::dot(dRefCol, dRefCol) * sampWeight;

                _refMeanDistBuffer[index] += glm::dvec2(dRef * sampWeight, sampWeight);
                _compatibilityBuffer[index] = refCompatibility(index);
                compatibility = _compatibilityBuffer[index];
            }


            // Mix current pixel with reference shot's own
            const glm::dvec4& refSamp = _referenceFilm.sampleBuffer[index];
            const glm::dvec2& refVar = _referenceFilm.varianceBuffer[index];
            glm::dvec2 mixedVar = newWeightedVar + refVar * compatibility;
            glm::dvec4 mixedSamp = newSample + refSamp * compatibility;


            double newDiv = 1.0;
            if(newSample.w >= _priorityWeightThreshold)
            {
                newDiv = toDivergence(
                    mixedSamp, mixedVar.x / mixedVar.y);
                _divergenceBuffer[index] = newDiv;
            }

            if(_colorOutput == ColorOutput::ALBEDO)
                _colorBuffer[index] = sampleToColor(mixedSamp);

            else if(_colorOutput == ColorOutput::WEIGHT)
                _colorBuffer[index] = weightToColor(newSample);

            else if(_colorOutput == ColorOutput::VARIANCE)
                _colorBuffer[index] = varianceToColor(mixedVar);

            else if(_colorOutput == ColorOutput::DIVERGENCE)
                _colorBuffer[index] = divergenceToColor(newDiv);

            else if(_colorOutput == ColorOutput::COMPATIBILITY)
                _colorBuffer[index] = compatibilityToColor(compatibility);
        }
        else
        {
            if(_colorOutput == ColorOutput::ALBEDO)
                _colorBuffer[index] = sampleToColor(newSample);

            else if(_colorOutput == ColorOutput::WEIGHT)
                _colorBuffer[index] = weightToColor(newSample);
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
        return glm::vec3(sample.w / 200.0);
    }

    glm::vec3 ConvergentFilm::divergenceToColor(double divergence) const
    {
        return glm::vec3(divergence * 7.5);
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
        if(priority < _priorityThreshold)
        {
            return glm::vec3(priority / (3.0 * _priorityThreshold));
        }
        else
        {
            // These pixels will be raytraced
            double d = (priority - _priorityThreshold);
            double q = d / (d + 0.5);
            return glm::dvec3(
                // Red
                glm::smoothstep(0.5, 0.75,  q),
                // Green
                glm::smoothstep(0.0, 0.25, q) - glm::smoothstep(0.75, 1.0, q),
                // Blue
                1.0 - glm::smoothstep(0.25, 0.5, q));
        }
    }

    glm::vec3 ConvergentFilm::compatibilityToColor(double compatibility) const
    {
        return glm::vec3(compatibility);
    }

    double ConvergentFilm::toDivergence(
            const glm::dvec4& sample,
            double variance) const
    {
        const double WEIGHT_OFFSET = 0.04;

        glm::dvec3 color = sampleToColor(sample);
        glm::dvec3 clamped = glm::min(color, _maxPixelIntensity);
        double scale = glm::length(clamped) + WEIGHT_OFFSET;
        return glm::sqrt(variance) * (scale + 1.0) / (scale * sample.w);
    }

    double ConvergentFilm::toPriority(
            const glm::dvec4& sample,
            double variance) const
    {
        double weight = sample.w;
        double weight3 = weight*weight*weight;
        double div = toDivergence(sample, variance);

        return _priorityScale * (div + _priorityWeightBias / weight3);
    }

    double ConvergentFilm::refCompatibility(unsigned int index) const
    {
        const glm::dvec4& refSamp = _referenceFilm.sampleBuffer[index];

        if(refSamp.w == 0)
            return 0.0;

        const glm::dvec4& curSamp = _sampleBuffer[index];
        const glm::dvec2& curVar= _varianceBuffer[index];
        double var = curVar.x / curVar.y;

        glm::dvec3 refCol = sampleToColor(refSamp);
        glm::dvec3 curCol = sampleToColor(curSamp);
        glm::dvec3 minCol = glm::min(refCol, curCol);
        double signal2 = glm::dot(minCol, minCol);

        double refVar = _refMeanDistBuffer[index].x /
                _refMeanDistBuffer[index].y;
        double noise2 = var - refVar;

        double signalToNoise = glm::sqrt(noise2 / signal2);

        double weightRatio = glm::min(2.0 * curSamp.w / (curSamp.w + refSamp.w), 1.0);
        return glm::clamp(((weightRatio + 1.0) / (1.0 + signalToNoise * _condifdenceRange) - 1.0), 0.0, 1.0);
    }
}
