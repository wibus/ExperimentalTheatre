#include "NetworkFilm.h"

#include <CellarWorkbench/Misc/Log.h>

#include "../Network/TileMessage.h"

using namespace cellar;


namespace prop3
{
    NetworkFilm::NetworkFilm() :
        _maxDataRateAvailable(2.8e6)
    {

    }

    NetworkFilm::~NetworkFilm()
    {

    }

    const std::vector<glm::vec3>& NetworkFilm::colorBuffer(ColorOutput colorOutput)
    {
        return _colorBuffer;
    }

    void NetworkFilm::clear(const glm::dvec3& color, bool hardReset)
    {
        size_t pixelCount = _frameResolution.x * _frameResolution.y;

        _newTileCompleted = false;
        _newFrameCompleted = false;

        _nextTileId = 0;
        _framePassCount = 0;
        _priorityThreshold = 1.0;
        _avrgPixelPriority = 1.0;
        _tileCompletedCount = 0;
        _cumulatedTileByteCount = 0;
        _startTime = std::chrono::high_resolution_clock::now();

        while(!_tileMsgs.empty())
            _tileMsgs.pop();

        _sampleBuffer.clear();
        glm::dvec4 sample(color, 0.0);
        _sampleBuffer.resize(pixelCount, sample);

        if(hardReset)
        {
            size_t pixelCount = _frameResolution.x * _frameResolution.y;

            _colorBuffer.clear();
            _colorBuffer.resize(pixelCount, color);

            _depthBuffer.clear();
            _depthBuffer.resize(pixelCount, INFINITY);
        }
    }

    void NetworkFilm::backupAsReferenceShot()
    {
        // Do nothing
    }

    bool NetworkFilm::saveReferenceShot(const std::string& name) const
    {
        return false;
    }

    bool NetworkFilm::loadReferenceShot(const std::string& name)
    {
        return false;
    }

    bool NetworkFilm::clearReferenceShot()
    {
        return false;
    }

    bool NetworkFilm::saveRawFilm(const std::string& name) const
    {
        return false;
    }

    bool NetworkFilm::loadRawFilm(const std::string& name)
    {
        return false;
    }

    double NetworkFilm::compileDivergence() const
    {
        return 1.0;
    }

    void NetworkFilm::tileCompleted(Tile& tile)
    {
        _newTileCompleted = true;
        ++_tileCompletedCount;

        std::shared_ptr<TileMessage> msg(
            new TileMessage(*this, tile.tileId(), stateUid()));

        msg->encode();

        if(msg->isValid())
        {
            addOutgoingTile(msg);
            _cumulatedTileByteCount += msg->size();
        }

        if(_tileCompletedCount == _tiles.size())
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> sec = endTime - _startTime;
            double dataRate = _cumulatedTileByteCount / sec.count();

            int kBPerSec = dataRate / 1000;

            if(dataRate > _maxDataRateAvailable)
            {
                _avrgPixelPriority = _avrgPixelPriority * dataRate
                                         / _maxDataRateAvailable;

                getLog().postMessage(new Message('W', false,
                    "NetworkFilm uses to much bandwidth (" +
                    std::to_string(kBPerSec) + "kB/s); "\
                    "increasing pixel priorities to " +
                    std::to_string(_avrgPixelPriority),
                    "NetworkFilm"));
            }
            else
            {
                getLog().postMessage(new Message('W', false,
                    "NetworkFilm uses acceptable bandwidth (" +
                    std::to_string(kBPerSec) + "kB/s)",
                    "NetworkFilm"));
            }
        }
    }

    std::shared_ptr<TileMessage> NetworkFilm::nextOutgoingTile()
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

    void NetworkFilm::addOutgoingTile(const std::shared_ptr<TileMessage>& msg)
    {
        _tileMsgMutex.lock();
        _tileMsgs.push(msg);
        _tileMsgMutex.unlock();
    }

    void NetworkFilm::endTileReached()
    {
        _cvMutex.lock();
        _nextTileId = 0;
        _newFrameCompleted = true;
        _cvMutex.unlock();
        _cv.notify_all();
    }

    double NetworkFilm::pixelDivergence(int index) const
    {
        return 0.0;
    }

    double NetworkFilm::pixelPriority(int index) const
    {
        return _avrgPixelPriority;
    }

    glm::dvec4 NetworkFilm::pixelSample(int index) const
    {
        return _sampleBuffer[index];
    }

    void NetworkFilm::addSample(int index, const glm::dvec4& sample)
    {
        _sampleBuffer[index] = sample;

        _colorBuffer[index] =
                glm::dvec3(_sampleBuffer[index]) /
                    _sampleBuffer[index].w;
    }
}
