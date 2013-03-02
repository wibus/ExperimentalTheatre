#ifndef QTCLOCK_H
#define QTCLOCK_H

#include "AbstractClock.h"

class QElapsedTimer;


namespace cellar
{
    class CELLAR_EXPORT QtClock : public AbstractClock
    {
    public:
        QtClock();
        explicit QtClock(float evalInterval);
        ~QtClock();

        virtual void start();
        virtual float tick();
        virtual float reset();
        virtual float stop();

    private:
        QElapsedTimer* _qtTimer;
        unsigned int _lastRawSample;
        unsigned int _nbSamples;
        float _samplesSum;
        float _evalInterval;

        static const unsigned int CLOCKS_PER_SECOND;
    };
}

#endif // QTCLOCK_H
