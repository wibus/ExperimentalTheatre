#include "NetworkFilm.h"

#include <CellarWorkbench/Misc/Log.h>

#include "../Network/TileMessage.h"

using namespace cellar;


namespace prop3
{
    NetworkFilm::NetworkFilm() :
        _initialPixelPriority(1.5),
        _maxDataRateAvailable(2.8e6),
        _cumulatedByteCountThreshold(3e6)
    {

    }

    NetworkFilm::~NetworkFilm()
    {

    }

    const std::vector<glm::vec3>& NetworkFilm::colorBuffer(ColorOutput colorOutput)
    {
        return _colorBuffer;
    }

    void NetworkFilm::resetFilmState()
    {
        _newTileCompleted = false;
        _newFrameCompleted = false;

        _nextTileId = 0;
        _framePassCount = 0;
        _tileCompletedCount = 0;
        _priorityThreshold = 1.0;
        _currentPixelPriority = _initialPixelPriority;
        _startTime = std::chrono::high_resolution_clock::now();
        _cumulatedTileByteCount = 0;
        _bandwithOptimized = false;

        while(!_tileMsgs.empty())
            _tileMsgs.pop();
    }

    void NetworkFilm::clearBuffers(const glm::dvec3& color)
    {
        size_t pixelCount = _frameResolution.x * _frameResolution.y;

        _sampleBuffer.clear();
        _sampleBuffer.resize(pixelCount, glm::dvec4(0));

        _colorBuffer.clear();
        _colorBuffer.resize(pixelCount, color);
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
        ++_tileCompletedCount;

        std::shared_ptr<TileMessage> msg(
            new TileMessage(*this, tile.tileId(), stateUid()));

        msg->encode();

        if(msg->isValid())
        {
            addOutgoingTile(msg);
        }

        if((_tileCompletedCount & tileCount()) == 0)
        {
            _cvMutex.lock();
            ++_framePassCount;
            _newFrameCompleted = true;
            _cvMutex.unlock();
            _cv.notify_all();
        }

        _newTileCompleted = true;
    }

    void NetworkFilm::rewindTiles()
    {
        _nextTileId = 0;
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
        _cumulatedTileByteCount += msg->size();

        if(!_bandwithOptimized &&
           _cumulatedTileByteCount > _cumulatedByteCountThreshold)
        {
            _bandwithOptimized = true;
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> sec = endTime - _startTime;
            double dataRate = _cumulatedTileByteCount / sec.count();

            int kBPerSec = dataRate / 1000;

            _currentPixelPriority = _currentPixelPriority *
                glm::sqrt(dataRate / _maxDataRateAvailable);
            _currentPixelPriority = glm::max(_currentPixelPriority, 1.0);

            getLog().postMessage(new Message('I', false,
                "Optimizing bandwith (" +
                std::to_string(kBPerSec) + "kB/s); "\
                "new pixel priority is " +
                std::to_string(_currentPixelPriority),
                "NetworkFilm"));
        }

        _tileMsgMutex.unlock();
    }

    void NetworkFilm::endTileReached()
    {
        _nextTileId = 0;
    }

    double NetworkFilm::pixelDivergence(int index) const
    {
        return 0.0;
    }

    double NetworkFilm::pixelPriority(int index) const
    {
        return _currentPixelPriority;
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
