#ifndef CELLARWORKBENCH_SPECIFIC_OBSERVER_H
#define CELLARWORKBENCH_SPECIFIC_OBSERVER_H

#include "../libCellarWorkbench_global.h"


namespace cellar
{
    template <typename Msg>
    class SpecificObserver
    {
    protected:
        SpecificObserver();

    public:
        virtual ~SpecificObserver();
        virtual void notify(Msg& msg) = 0;
    };



    // IMPLEMENTATION //
    template <typename Msg>
    SpecificObserver<Msg>::SpecificObserver()
        {}

    template <typename Msg>
    SpecificObserver<Msg>::~SpecificObserver()
        {}
}

#endif // CELLARWORKBENCH_SPECIFIC_OBSERVER_H
