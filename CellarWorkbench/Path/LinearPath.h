#ifndef CELLAR_LINEARPATH_H
#define CELLAR_LINEARPATH_H

#include "AbstractPath.h"


namespace cellar
{
    template<typename Data>
    class CELLAR_EXPORT LinearPath : public AbstractPath<Data>
    {
    public:
        LinearPath(const Data& begin, const Data& end);
        virtual ~LinearPath();


        Data begin() const;

        void setBegin(const Data& begin);

        Data end() const;

        void setEnd(const Data& begin);


        virtual Data value(double t) override;
        virtual Data tangent(double t) override;
        virtual Data curvature(double t) override;


    private:
        Data _begin;
        Data _end;
    };



    // IMPLEMENTATION
    template<typename Data>
    LinearPath<Data>::LinearPath(const Data& begin, const Data& end) :
        _begin(begin),
        _end(end)
    {

    }

    template<typename Data>
    LinearPath<Data>::~LinearPath()
    {

    }

    template<typename Data>
    inline Data LinearPath<Data>::begin() const
    {
        return _begin;
    }

    template<typename Data>
    inline void LinearPath<Data>::setBegin(const Data& begin)
    {
        _begin = begin;
    }

    template<typename Data>
    inline Data LinearPath<Data>::end() const
    {
        return _end;
    }

    template<typename Data>
    inline void LinearPath<Data>::setEnd(const Data& begin)
    {
        _end = end;
    }

    template<typename Data>
    Data LinearPath<Data>::value(double t)
    {
        return (1.0-t) * _begin + t * _end;
    }

    template<typename Data>
    Data LinearPath<Data>::tangent(double t)
    {
        return _end - _begin;
    }

    template<typename Data>
    Data LinearPath<Data>::curvature(double t)
    {
        return Data(0);
    }
}

#endif // CELLAR_LINEARPATH_H
