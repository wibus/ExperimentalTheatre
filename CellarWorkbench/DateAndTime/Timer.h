#ifndef CELLARWORKBENCH_TIMER_H
#define CELLARWORKBENCH_TIMER_H

#include <ctime>

#include "../libCellarWorkbench_global.h"


namespace cellar
{
    class CELLAR_EXPORT Timer
    {
    public :
        Timer();

        void  start();
        float stop();
        void  setCounter(float seconds);
        float counter() const;

    private :
        unsigned int _counter;
        bool _isCounterActive;
    };



    // IMPLEMENTATION //
    inline void Timer::start()
    {
        _counter = clock();
        _isCounterActive = true;
    }

    inline float Timer::stop()
    {
        if(_isCounterActive)
        {
                unsigned int position = clock() - _counter;
                _counter = 0;
                _isCounterActive = false;
                return position / (float)CLOCKS_PER_SEC;
        }
        //else
        return 0.0f;
    }

    inline void Timer::setCounter(float seconds)
    {
        _counter = clock() - static_cast<int>(seconds) * CLOCKS_PER_SEC;
        _isCounterActive = true;
    }

    inline float Timer::counter() const
    {
        if(_isCounterActive)
            return (clock() - _counter) / (float)CLOCKS_PER_SEC;

        return 0.0f;
    }
}

#endif // CELLARWORKBENCH_TIMER_H
