#ifndef CELLARWORKBENCH_SINGLETON_H
#define CELLARWORKBENCH_SINGLETON_H

#include "libCellarWorkbench_global.h"


namespace cellar
{
    template < typename T >
    class Singleton
    {
    public :
        static T* getInstance();
        static void killInstance();

    protected :
        Singleton();
        virtual ~Singleton();

    private :
        Singleton(const Singleton&);
        Singleton& operator=(const Singleton&);

        static T* _instance;
    };



    // IMPLEMENTATION //

    template < typename T >
    T* Singleton<T>::_instance = 0x0;

    template < typename T >
    T* Singleton<T>::getInstance()
    {
        if(_instance == 0x0)
            _instance = new T();
        return _instance;
    }

    template < typename T >
    void Singleton<T>::killInstance()
    {
        if(_instance != 0)
        {
            delete _instance;
            _instance = 0;
        }
    }

    template < typename T >
    Singleton<T>::Singleton()
        {}

    template < typename T >
    Singleton<T>::~Singleton()
        {}

    template < typename T >
    Singleton<T>::Singleton(const Singleton&)
        {}

    template < typename T >
    Singleton<T>& Singleton<T>::operator=(const Singleton&)
        {return *this;}
}

#endif // CELLARWORKBENCH_SINGLETON_H_
