#ifndef SCAENA_STAGE_TIME_H
#define SCAENA_STAGE_TIME_H

#include "libScaena_global.h"


namespace scaena
{
    class SCAENA_EXPORT StageTime
    {
    public :
        StageTime(float elapsedTime,
                  float totalTime,
                  bool  isSlow = false);

        float elapsedTime() const;
        float totalTime()   const;
        bool  isSlow()      const;

    private :
        float _elapsedTime;
        float _totalTime;
        bool  _isSlow;
    };



    // IMPLEMENTATION //
    inline float StageTime::elapsedTime() const
        {return _elapsedTime;}

    inline float StageTime::totalTime() const
        {return _totalTime;}

    inline bool StageTime::isSlow() const
        {return _isSlow;}
}

#endif // SCAENA_STAGE_TIME_H
