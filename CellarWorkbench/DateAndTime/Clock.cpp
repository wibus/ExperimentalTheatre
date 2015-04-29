#include "Clock.h"

namespace cellar
{
    const float Clock::AGE_RATIO = 0.50;

    Clock::Clock() :
        _isRunning(false),
        _totalSeconds(0.0f),
        _elapsedSeconds(0.0f),
        _ticksPerSecond(0.0f),
        _nbSamples(0),
        _samplesSum(0.0f),
        _evalInterval(0.30f)
    {
    }

    Clock::Clock(float evalInterval) :
        _isRunning(false),
        _totalSeconds(0.0f),
        _elapsedSeconds(0.0f),
        _ticksPerSecond(0.0f),
        _nbSamples(0),
        _samplesSum(0.0f),
        _evalInterval(evalInterval)
    {
    }

    void Clock::start()
    {
        if(!_isRunning)
        {
            _isRunning = true;
            _totalSeconds = 0.0f;
            _lastRawSample = std::chrono::high_resolution_clock::now();
        }
    }

    float Clock::stop()
    {
        tick();
        _isRunning = false;
        return _totalSeconds;
    }

    float Clock::tick()
    {
        if(!_isRunning)
        {
            return 0.0f;
        }

        TimePoint sample = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> seconds = (sample - _lastRawSample);
        _elapsedSeconds = seconds.count();

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

    void Clock::reset()
    {
        _elapsedSeconds = 0.0f;
        _ticksPerSecond = 0.0f;
        _totalSeconds = 0.0f;

        _samplesSum = 0.0f;
        _nbSamples = 0;
    }
}
