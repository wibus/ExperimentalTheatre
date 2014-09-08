#include "CClock.h"
#include <ctime>


namespace cellar
{
    CClock::CClock() :
        _lastRawSample(0),
        _nbSamples(0),
        _samplesSum(0.0f),
        _evalInterval(0.30f)
    {
    }

    CClock::CClock(float evalInterval) :
        _lastRawSample(0),
        _nbSamples(0),
        _samplesSum(0.0f),
        _evalInterval(evalInterval)
    {
    }

    void CClock::start()
    {
        if(!_isRunning)
        {
            _isRunning = true;
            _totalSeconds = 0.0f;
            _lastRawSample = clock();
        }
    }

    float CClock::tick()
    {
        if(!_isRunning)
        {
            return 0.0f;
        }

        unsigned long long sample = clock();
        _elapsedSeconds = (sample - _lastRawSample) /
                        static_cast<float>(CLOCKS_PER_SEC);

        _lastRawSample = sample;
        _totalSeconds += _elapsedSeconds;
        _samplesSum += _elapsedSeconds;
        ++_nbSamples;

        if(!_ticksPerSecond)
        {
            _ticksPerSecond = _nbSamples / _samplesSum;
            _samplesSum = 0.0f;
            _nbSamples = 0;
        }
        else if(_evalInterval <= _samplesSum)
        {
            _ticksPerSecond = (1 - AGE_RATIO) * _ticksPerSecond +
                              AGE_RATIO * _nbSamples / _samplesSum;
            _samplesSum = 0.0f;
            _nbSamples = 0;
        }

        return _elapsedSeconds;
    }

    void CClock::reset()
    {
        AbstractClock::reset();
        _lastRawSample = 0;
        _samplesSum = 0.0f;
        _nbSamples = 0;
    }
}
