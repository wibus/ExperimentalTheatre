#ifndef CELLARWORKBENCH_SPECIFICSUBJECT_H
#define CELLARWORKBENCH_SPECIFICSUBJECT_H

#include <vector>

#include "../libCellarWorkbench_global.h"
#include "SpecificObserver.h"


namespace cellar
{

    template <typename Msg>
    class SpecificSubject
    {
    protected:
        SpecificSubject();

    public:
        virtual ~SpecificSubject();
        bool registerObserver(SpecificObserver<Msg>& obs);
        bool unregisterObserver(const SpecificObserver<Msg>& obs);

        void commitExternalChanges(Msg& msg);

    protected:
        void notifyObservers(Msg& msg);

    private:
        std::vector< SpecificObserver<Msg>* > _observers;
    };



    // IMPLEMENTATION //
    template <typename Msg>
    SpecificSubject<Msg>::SpecificSubject() :
        _observers()
    {}

    template <typename Msg>
    SpecificSubject<Msg>::~SpecificSubject()
    {}

    template <typename Msg>
    bool SpecificSubject<Msg>::registerObserver(SpecificObserver<Msg>& obs)
    {
        for(unsigned int i=0; i < _observers.size(); ++i)
            if(_observers[i] == &obs)
                return false;

        _observers.push_back(&obs);
        return true;
    }

    template <typename Msg>
    bool SpecificSubject<Msg>::unregisterObserver(const SpecificObserver<Msg>& obs)
    {
        typename std::vector< SpecificObserver<Msg>* >::iterator obsIt = _observers.begin();
        for(; obsIt != _observers.end(); ++obsIt)
            if(*obsIt == &obs)
            {
                _observers.erase(obsIt);
                return true;
            }

        return false;
    }

    template <typename Msg>
    inline void SpecificSubject<Msg>::commitExternalChanges(Msg& msg)
    {
        notifyObservers( msg );
    }

    template <typename Msg>
    void SpecificSubject<Msg>::notifyObservers(Msg& msg)
    {
        for(unsigned int i=0; i < _observers.size(); ++i)
            _observers[i]->notify(msg);
    }
}

#endif // CELLARWORKBENCH_SPECIFICSUBJECT_H
