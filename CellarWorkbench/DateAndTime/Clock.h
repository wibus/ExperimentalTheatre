#ifndef CELLARWORKBENCH_CLOCK_H
#define CELLARWORKBENCH_CLOCK_H

#include <chrono>

#include "../libCellarWorkbench_global.h"


namespace cellar
{
    class CELLAR_EXPORT Clock
    {
    public:
        Clock();
        explicit Clock(float evalInterval);

        void start();
        float stop();
        float tick();
        void reset();

        bool isRunning() const;
        float totalSeconds() const;
        float elapsedSeconds() const;
        float ticksPerSecond() const;

        static const float AGE_RATIO;

    private:
        bool _isRunning;
        float _totalSeconds;
        float _elapsedSeconds;
        float _ticksPerSecond;

        typedef std::chrono::time_point<std::chrono::high_resolution_clock>
            TimePoint;

        TimePoint _lastRawSample;
        unsigned int _nbSamples;
        float _samplesSum;
        float _evalInterval;
    };


    // IMPLEMENTATION //
    inline bool Clock::isRunning() const {return _isRunning;}
    inline float Clock::totalSeconds() const {return _totalSeconds;}
    inline float Clock::elapsedSeconds() const {return _elapsedSeconds;}
    inline float Clock::ticksPerSecond() const {return _ticksPerSecond;}
}

#endif // CELLARWORKBENCH_CLOCK_H
