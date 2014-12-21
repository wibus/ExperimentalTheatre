#ifndef CELLARWORKBENCH_SPECIFIC_SUBJECT_H
#define CELLARWORKBENCH_SPECIFIC_SUBJECT_H

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
        void setIsChanged(bool is);
        bool isChanged() const;
        void notifyObservers(Msg& msg);

    private:
        bool _isChanged;
        std::vector< SpecificObserver<Msg>* > _observers;
    };



    // IMPLEMENTATION //
    template <typename Msg>
    SpecificSubject<Msg>::SpecificSubject() :
        _isChanged(false),
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
        setIsChanged( true );
        notifyObservers( msg );
    }

    template <typename Msg>
    inline void SpecificSubject<Msg>::setIsChanged(bool is)
        {_isChanged = is;}

    template <typename Msg>
    inline bool SpecificSubject<Msg>::isChanged() const
        {return _isChanged;}

    template <typename Msg>
    void SpecificSubject<Msg>::notifyObservers(Msg& msg)
    {
        if(_isChanged)
            for(unsigned int i=0; i < _observers.size(); ++i)
                _observers[i]->notify(msg);

        setIsChanged( false );
    }
}

#endif // CELLARWORKBENCH_SPECIFIC_SUBJECT_H
