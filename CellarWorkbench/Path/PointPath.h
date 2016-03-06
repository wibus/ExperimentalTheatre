#ifndef CELLAR_POINTPATH_H
#define CELLAR_POINTPATH_H

#include "AbstractPath.h"
#include "PathVisitor.h"


namespace cellar
{
    template<typename Data>
    class CELLAR_EXPORT PointPath : public LeafPath<Data>
    {
    public:
        PointPath(double duration, const Data& value);
        virtual ~PointPath();


        Data& value();

        Data value() const;

        void setValue(const Data& value);


        virtual Data value(double t) const override;
        virtual Data tangent(double t) const override;
        virtual Data curvature(double t) const override;

        virtual void accept(PathVisitor<Data>& visitor) override;

        virtual void update() override;


    private:
        Data _value;
    };



    // IMPLEMENTATION
    template<typename Data>
    PointPath<Data>::PointPath(
            double duration,
            const Data& value) :
        LeafPath<Data>(duration),
        _value(value)
    {

    }

    template<typename Data>
    PointPath<Data>::~PointPath()
    {

    }

    template<typename Data>
    inline Data& PointPath<Data>::value()
    {
        return _value;
    }

    template<typename Data>
    inline Data PointPath<Data>::value() const
    {
        return _value;
    }

    template<typename Data>
    inline void PointPath<Data>::setValue(const Data& value)
    {
        _value = value;
    }

    template<typename Data>
    Data PointPath<Data>::value(double t) const
    {
        return _value;
    }

    template<typename Data>
    Data PointPath<Data>::tangent(double t) const
    {
        return Data(0);
    }

    template<typename Data>
    Data PointPath<Data>::curvature(double t) const
    {
        return Data(0);
    }

    template<typename Data>
    void PointPath<Data>::accept(PathVisitor<Data>& visitor)
    {
        visitor.visit(*this);
    }

    template<typename Data>
    void PointPath<Data>::update()
    {

    }
}

#endif // CELLAR_POINTPATH_H
