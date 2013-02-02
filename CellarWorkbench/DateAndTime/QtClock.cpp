#include "QtClock.h"
#include <QElapsedTimer>


namespace cellar
{
    const unsigned int QtClock::CLOCKS_PER_SECOND = 1000;

    QtClock::QtClock() :
        _qtTimer(new QElapsedTimer()),
        _lastRawSample(0),
        _nbSamples(0),
        _samplesSum(0.0f),
        _evalInterval(0.30f)
    {
    }

    QtClock::QtClock(float evalInterval) :
        _qtTimer(new QElapsedTimer()),
        _lastRawSample(0),
        _nbSamples(0),
        _samplesSum(0.0f),
        _evalInterval(evalInterval)
    {
    }

    QtClock::~QtClock()
    {
        delete _qtTimer;
    }

    void QtClock::start()
    {
        _isRunning = true;
        _totalSeconds = 0.0f;
        _qtTimer->start();
    }

    float QtClock::tick()
    {
        float elapsed = (_qtTimer->elapsed() - _lastRawSample) / (float) CLOCKS_PER_SECOND;

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

        _lastRawSample = _qtTimer->elapsed();
        return elapsed;
    }

    float QtClock::reset()
    {
        float elapsed = (_qtTimer->elapsed() - _lastRawSample) / (float) CLOCKS_PER_SECOND;
        _secondsPerTick = 0.0f;
        _ticksPerSecond = 0.0f;
        _nbSamples = 0;
        _samplesSum = 0.0f;

        _lastRawSample = _qtTimer->elapsed();
        return (_totalSeconds += elapsed);
    }

    float QtClock::stop()
    {
        _isRunning = false;
        _totalSeconds += (_qtTimer->elapsed() - _lastRawSample) / (float) CLOCKS_PER_SECOND;
        _secondsPerTick = 0.0f;
        _ticksPerSecond = 0.0f;
        _nbSamples = 0;
        _samplesSum = 0.0f;

        return _totalSeconds;
    }
}
