#include "AbstractClock.h"

namespace cellar
{
    const float AbstractClock::AGE_RATIO = 0.50;

    AbstractClock::AbstractClock() :
        _isRunning(false),
        _totalSeconds(0.0f),
        _secondsPerTick(0.0f),
        _ticksPerSecond(0.0f)
    {
    }

    AbstractClock::~AbstractClock()
    {
    }
}
