#ifndef CELLAR_ABSTARCTPATH_H
#define CELLAR_ABSTARCTPATH_H

#include <GLM/glm.hpp>

#include <CellarWorkbench/libCellarWorkbench_global.h>


namespace cellar
{
    template<typename Data>
    class CELLAR_EXPORT AbstractPath
    {
    protected:
        AbstractPath();

    public:
        virtual ~AbstractPath();

        virtual Data value(double t) const = 0;
        virtual Data tangent(double t) const = 0;
        virtual Data curvature(double t) const = 0;

    private:
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
}

#endif // CELLAR_ABSTARCTPATH_H
