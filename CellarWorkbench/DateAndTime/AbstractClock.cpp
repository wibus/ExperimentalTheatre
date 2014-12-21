#include "AbstractClock.h"


namespace cellar
{
    const float AbstractClock::AGE_RATIO = 0.50;

    AbstractClock::AbstractClock() :
        _isRunning(false),
        _totalSeconds(0.0f),
        _elapsedSeconds(0.0f),
        _ticksPerSecond(0.0f)
    {
    }

    AbstractClock::~AbstractClock()
    {
    }

    void AbstractClock::reset()
    {
        _elapsedSeconds = 0.0f;
        _ticksPerSecond = 0.0f;
        _totalSeconds = 0.0f;
    }

    float AbstractClock::stop()
    {
        tick();
        _isRunning = false;
        return _totalSeconds;
    }
}
