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
        if(!_isRunning)
        {
            _isRunning = true;
            _totalSeconds = 0.0f;
            _lastRawSample = _qtTimer->elapsed();
            _qtTimer->start();
        }
    }

    float QtClock::tick()
    {
        if(!_isRunning)
        {
            return 0.0f;
        }

        unsigned long long sample = _qtTimer->elapsed();
        _elapsedSeconds = (sample - _lastRawSample) /
                        static_cast<float>(CLOCKS_PER_SECOND);

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

    void QtClock::reset()
    {
        AbstractClock::reset();
        _lastRawSample = 0;
        _samplesSum = 0.0f;
        _nbSamples = 0;
    }
}
