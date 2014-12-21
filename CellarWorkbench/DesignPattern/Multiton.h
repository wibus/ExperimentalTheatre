#ifndef CELLARWORKBENCH_MULTITON_H
#define CELLARWORKBENCH_MULTITON_H

#include <stdexcept>

#include "../libCellarWorkbench_global.h"


namespace cellar
{
    template <typename T, size_t N>
    class Multiton
    {
    public:
        static T* getPtrTo(size_t n);
        static T& getRefTo(size_t n);
        static void resetAll();

    protected:
        Multiton();
        virtual ~Multiton();

    private:
        Multiton(const Multiton&);
        Multiton& operator=(const Multiton&);

        static T* _instances;
    };




    // IMPLEMENTATION //
    template <typename T, size_t N>
    inline T* Multiton<T, N>::getPtrTo(size_t n)
    {
        return &getRefTo(n);
    }

    template <typename T, size_t N>
    inline T& Multiton<T, N>::getRefTo(size_t n)
    {
        if(0 > n || n >= N)
            throw std::invalid_argument("Multiton index out of bounds");

        return _instances[n];
    }

    template <typename T, size_t N>
    inline void Multiton<T, N>::resetAll()
    {
        delete _instances;
        _instances = new T[N];
    }

    template <typename T, size_t N>
    Multiton<T, N>::Multiton()
    {
    }

    template <typename T, size_t N>
    Multiton<T, N>::~Multiton()
    {
    }

    template <typename T, size_t N>
    Multiton<T, N>::Multiton(const Multiton&)
    {
    }

    template <typename T, size_t N>
    Multiton<T, N>& Multiton<T, N>::operator=(const Multiton&)
    {
    }

    template <typename T, size_t N>
    T* Multiton<T, N>::_instances = new T[N];
}

#endif // CELLARWORKBENCH_MULTITON_H
