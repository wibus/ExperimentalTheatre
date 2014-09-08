#include "StageTime.h"


namespace scaena
{
    StageTime::StageTime(float totalTime,
                         float elapsedTime,
                         float framePerSecond,
                         bool isSlow) :
        _isSlow(isSlow),
        _totalTime(totalTime),
        _elapsedTime(elapsedTime),
        _framePerSecond(framePerSecond)
    {}
}
