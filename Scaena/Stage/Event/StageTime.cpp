#include "StageTime.h"


namespace scaena
{
    StageTime::StageTime(float totalTime,
                         float elapsedTime,
                         float framesPerSecond,
                         bool isSlow) :
        _isSlow(isSlow),
        _totalTime(totalTime),
        _elapsedTime(elapsedTime),
        _framesPerSecond(framesPerSecond)
    {}
}
