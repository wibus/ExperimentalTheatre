#ifndef SCAENA_STAGE_TIME_H
#define SCAENA_STAGE_TIME_H

#include "libScaena_global.h"


namespace scaena
{
    class SCAENA_EXPORT StageTime
    {
    public :
        StageTime(float totalTime,
                  float elapsedTime,
                  float framePerSecond,
                  bool  isSlow = false);

        bool  isSlow() const;
        float totalTime() const;
        float elapsedTime() const;
        float framePerSecond() const;

    private :
        bool  _isSlow;
        float _totalTime;
        float _elapsedTime;
        float _framePerSecond;
    };



    // IMPLEMENTATION //
    inline bool StageTime::isSlow() const
    {
        return _isSlow;
    }

    inline float StageTime::totalTime() const
    {
        return _totalTime;
    }

    inline float StageTime::elapsedTime() const
    {
        return _elapsedTime;
    }

    inline float StageTime::framePerSecond() const
    {
        return _framePerSecond;
    }
}

#endif // SCAENA_STAGE_TIME_H
