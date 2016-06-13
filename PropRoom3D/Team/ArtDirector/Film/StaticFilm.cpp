#include "StaticFilm.h"


namespace prop3
{
    StaticFilm::StaticFilm()
    {

    }

    StaticFilm::~StaticFilm()
    {

    }

    const std::vector<glm::vec3>& StaticFilm::colorBuffer(ColorOutput colorOutput)
    {
        return _colorBuffer;
    }

    void StaticFilm::resetFilmState()
    {
        _tileCompletedCount = 0;
        _newTileCompleted = false;
        _newFrameCompleted = false;

        _nextTileId = 0;
        _framePassCount = 0;
        _priorityThreshold = 0.0;
    }

    void StaticFilm::clearBuffers(const glm::dvec3& color)
    {
        size_t pixelCount = _frameResolution.x * _frameResolution.y;
        if(pixelCount != _colorBuffer.size())
        {
            _colorBuffer.clear();
            _colorBuffer.resize(pixelCount, color);

            _depthBuffer.clear();
            _depthBuffer.resize(pixelCount, INFINITY);
        }
    }

    void StaticFilm::backupAsReferenceShot()
    {
        // Do nothing
    }

    bool StaticFilm::saveReferenceShot(const std::string& name) const
    {
        return false;
    }

    bool StaticFilm::loadReferenceShot(const std::string& name)
    {
        return false;
    }

    bool StaticFilm::clearReferenceShot()
    {
        return false;
    }

    bool StaticFilm::saveRawFilm(const std::string& name) const
    {
        return false;
    }

    bool StaticFilm::loadRawFilm(const std::string& name)
    {
        return false;
    }

    double StaticFilm::compileDivergence() const
    {
        return 1.0;
    }

    bool StaticFilm::needNewTiles() const
    {
        return _nextTileId < _tiles.size();
    }

    void StaticFilm::tileCompleted(Tile& tile)
    {
        std::lock_guard<std::mutex> lk(_tilesMutex);

        ++_tileCompletedCount;
        if(_tileCompletedCount >= _tiles.size())
        {
            _cvMutex.lock();
            _newFrameCompleted = true;
            ++_framePassCount;
            _cvMutex.unlock();
            _cv.notify_all();
        }

        _newTileCompleted = true;

    }

    void StaticFilm::rewindTiles()
    {
        _nextTileId = 0;
    }

    void StaticFilm::endTileReached()
    {
        // Do nothing
    }

    double StaticFilm::pixelDivergence(int index) const
    {
        return 0.0;
    }

    double StaticFilm::pixelPriority(int index) const
    {
        return 1.0;
    }

    glm::dvec4 StaticFilm::pixelSample(int index) const
    {
        return glm::dvec4(_colorBuffer[index], 1.0);
    }

    void StaticFilm::addSample(int index, const glm::dvec4& sample)
    {
        _colorBuffer[index] = glm::dvec3(sample);
        _depthBuffer[index] = sample.w;
    }
}
