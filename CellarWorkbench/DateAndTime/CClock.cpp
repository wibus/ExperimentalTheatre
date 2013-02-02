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
        _isRunning = true;
        _totalSeconds = 0.0f;
        _lastRawSample = clock();
    }

    float CClock::tick()
    {
        float elapsed = (clock() - _lastRawSample) / (float) CLOCKS_PER_SEC;

        _totalSeconds += elapsed;

        ++_nbSamples;
        _samplesSum += elapsed;

        if(_samplesSum >= _evalInterval)
        {
            if(_secondsPerTick == 0.0f)
                _secondsPerTick = _samplesSum / _nbSamples;

            _secondsPerTick = AGE_RATIO * _samplesSum / _nbSamples +
                              (1 - AGE_RATIO) * _secondsPerTick;
            _ticksPerSecond = 1.0f / _secondsPerTick;
            _nbSamples = 0;
            _samplesSum = 0.0f;
        }

        _lastRawSample = clock();
        return elapsed;
    }

    float CClock::reset()
    {
        float elapsed = (clock() - _lastRawSample) / (float) CLOCKS_PER_SEC;
        _secondsPerTick = 0.0f;
        _ticksPerSecond = 0.0f;
        _nbSamples = 0;
        _samplesSum = 0.0f;

        _lastRawSample = clock();
        return (_totalSeconds += elapsed);
    }

    float CClock::stop()
    {
        _isRunning = false;
        _totalSeconds += (clock() - _lastRawSample) / (float) CLOCKS_PER_SEC;
        _secondsPerTick = 0.0f;
        _ticksPerSecond = 0.0f;
        _nbSamples = 0;
        _samplesSum = 0.0f;

        return _totalSeconds;
    }
}
