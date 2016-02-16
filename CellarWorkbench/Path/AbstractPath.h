#ifndef CELLAR_ABSTARCTPATH_H
#define CELLAR_ABSTARCTPATH_H

#include <GLM/glm.hpp>

#include <CellarWorkbench/libCellarWorkbench_global.h>


namespace cellar
{
    template<typename Data>
    class PathVisitor;


    template<typename Data>
    class CELLAR_EXPORT AbstractPath
    {
    protected:
        AbstractPath();

    public:
        virtual ~AbstractPath();

        virtual double duration() const = 0;

        virtual Data value(double t) const = 0;
        virtual Data tangent(double t) const = 0;
        virtual Data curvature(double t) const = 0;

        virtual void accept(PathVisitor<Data>& visitor) = 0;

    private:
    };


    template<typename Data>
    class CELLAR_EXPORT LeafPath : public AbstractPath<Data>
    {
    protected:
        LeafPath(double duration = 1.0);

    public:
        virtual ~LeafPath();

        virtual double duration() const override;

        virtual void setDuration(double duration);

    private:
        double _duration;
    };



    // IMPLEMENTATION //
    template<typename Data>
    AbstractPath<Data>::AbstractPath()
    {

    }

    template<typename Data>
    AbstractPath<Data>::~AbstractPath()
    {

    }

    template<typename Data>
    LeafPath<Data>::LeafPath(double duration) :
        _duration(duration)
    {

    }

    template<typename Data>
    LeafPath<Data>::~LeafPath()
    {

    }

    template<typename Data>
    double LeafPath<Data>::duration() const
    {
        return _duration;
    }

    template<typename Data>
    void LeafPath<Data>::setDuration(double duration)
    {
        _duration = duration;
    }
}

#endif // CELLAR_ABSTARCTPATH_H
