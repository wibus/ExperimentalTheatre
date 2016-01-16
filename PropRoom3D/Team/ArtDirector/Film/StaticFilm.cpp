#include "StaticFilm.h"


namespace prop3
{
    StaticFilm::StaticFilm() :
        _tileCompletedCount(0)
    {

    }

    StaticFilm::~StaticFilm()
    {

    }

    void StaticFilm::clear(const glm::dvec3& color, bool hardReset)
    {
        _newTileCompleted = false;
        _newFrameCompleted = false;
        _tileCompletedCount = 0;

        _nextTileId = 0;
        _framePassCount = 0;
        _priorityThreshold = 0.0;

        if(hardReset)
        {
            size_t pixelCount = _frameResolution.x * _frameResolution.y;

            _colorBuffer.clear();
            _colorBuffer.resize(pixelCount, color);
        }
    }

    double StaticFilm::compileDivergence() const
    {
        return 1.0;
    }

    void StaticFilm::tileCompleted(Tile& tile)
    {
        std::lock_guard<std::mutex> lk(_tilesMutex);

        _newTileCompleted = true;
        ++_tileCompletedCount;

        if(_tileCompletedCount >= _tiles.size())
        {
            _cvMutex.lock();
            _newFrameCompleted = true;
            _cvMutex.unlock();
            _cv.notify_all();
        }
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

    void StaticFilm::setColor(int index, const glm::dvec3& color)
    {
        _colorBuffer[index] = color;
    }

    void StaticFilm::addSample(int index, const glm::dvec4& sample)
    {
        _colorBuffer[index] = glm::dvec3(sample);
    }
}
