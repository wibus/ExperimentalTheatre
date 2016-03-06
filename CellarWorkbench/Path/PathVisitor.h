#ifndef CELLAR_PATHVISITOR_H
#define CELLAR_PATHVISITOR_H

#include "AbstractPath.h"


namespace cellar
{
    template<typename Data>
    class PointPath;
    template<typename Data>
    class LinearPath;
    template<typename Data>
    class CubicSplinePath;
    template<typename Data>
    class BasisSplinePath;
    template<typename Data>
    class CompositePath;


    template<typename Data>
    class CELLAR_EXPORT PathVisitor
    {
    protected:
        PathVisitor();

    public:
        virtual ~PathVisitor();

        virtual void visit(PointPath<Data>& path) = 0;

        virtual void visit(LinearPath<Data>& path) = 0;

        virtual void visit(CubicSplinePath<Data>& path) = 0;

        virtual void visit(BasisSplinePath<Data>& path) = 0;

        virtual void visit(CompositePath<Data>& path) = 0;
    };



    // IMPLEMENTATION //
    template<typename Data>
    PathVisitor<Data>::PathVisitor()
    {

    }

    template<typename Data>
    PathVisitor<Data>::~PathVisitor()
    {

    }
}

#endif // CELLAR_PATHVISITOR_H
