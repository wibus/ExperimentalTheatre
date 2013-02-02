#include "StageTime.h"


namespace scaena
{
    StageTime::StageTime(float elapsedTime,
                         float totalTime,
                         bool isSlow) :
        _elapsedTime(elapsedTime),
        _totalTime(totalTime),
        _isSlow(isSlow)
    {}
}
