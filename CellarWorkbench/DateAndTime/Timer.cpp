#include "Timer.h"


namespace cellar
{
    Timer::Timer() :
        _counter(clock()),
        _isCounterActive(false)
    {
    }
}
