#ifndef CELLARWORKBENCH_CCLOCK_H
#define CELLARWORKBENCH_CCLOCK_H

#include "AbstractClock.h"

namespace cellar
{
    class CELLAR_EXPORT CClock : public AbstractClock
    {
    public:
        CClock();
        explicit CClock(float evalInterval);

        virtual void start();
        virtual float tick();
        virtual float reset();
        virtual float stop();

    private:
        unsigned int _lastRawSample;
        unsigned int _nbSamples;
        float _samplesSum;
        float _evalInterval;
    };
}

#endif // CELLARWORKBENCH_CCLOCK_H
