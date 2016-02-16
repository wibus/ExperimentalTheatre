#ifndef CELLAR_LINEARPATH_H
#define CELLAR_LINEARPATH_H

#include "AbstractPath.h"
#include "PathVisitor.h"


namespace cellar
{
    template<typename Data>
    class CELLAR_EXPORT LinearPath : public LeafPath<Data>
    {
    public:
        LinearPath(double duration, const Data& begin, const Data& end);
        virtual ~LinearPath();


        Data begin() const;

        void setBegin(const Data& begin);

        Data end() const;

        void setEnd(const Data& begin);


        virtual Data value(double t) const override;
        virtual Data tangent(double t) const override;
        virtual Data curvature(double t) const override;

        virtual void accept(PathVisitor<Data>& visitor) override;


    private:
        Data _begin;
        Data _end;
    };



    // IMPLEMENTATION
    template<typename Data>
    LinearPath<Data>::LinearPath(
            double duration,
            const Data& begin,
            const Data& end) :
        LeafPath<Data>(duration),
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
    Data LinearPath<Data>::value(double t) const
    {
        return (1.0-t) * _begin + t * _end;
    }

    template<typename Data>
    Data LinearPath<Data>::tangent(double t) const
    {
        return _end - _begin;
    }

    template<typename Data>
    Data LinearPath<Data>::curvature(double t) const
    {
        return Data(0);
    }

    template<typename Data>
    void LinearPath<Data>::accept(PathVisitor<Data>& visitor)
    {
        visitor.visit(*this);
    }
}

#endif // CELLAR_LINEARPATH_H
