#ifndef CELLARWORKBENCH_ABSTRACTCLOCK_H
#define CELLARWORKBENCH_ABSTRACTCLOCK_H

#include "../libCellarWorkbench_global.h"


namespace cellar
{
    class CELLAR_EXPORT AbstractClock
    {
    protected :
        AbstractClock();

    public:
        virtual ~AbstractClock();

        virtual void start() = 0;
        virtual float tick() = 0;
        virtual void reset();
        virtual float stop();

        bool isRunning() const;

        float totalSeconds() const;
        float elapsedSeconds() const;
        float ticksPerSecond() const;

        static const float AGE_RATIO;

    protected :
        bool _isRunning;
        float _totalSeconds;
        float _elapsedSeconds;
        float _ticksPerSecond;
    };


    // IMPLEMENTATION //
    inline bool AbstractClock::isRunning() const {return _isRunning;}
    inline float AbstractClock::totalSeconds() const {return _totalSeconds;}
    inline float AbstractClock::elapsedSeconds() const {return _elapsedSeconds;}
    inline float AbstractClock::ticksPerSecond() const {return _ticksPerSecond;}
}

#endif // CELLARWORKBENCH_ABSTRACTCLOCK_H
