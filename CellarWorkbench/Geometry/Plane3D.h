#ifndef CELLARWORKBENCH_PLANE3D_H
#define CELLARWORKBENCH_PLANE3D_H

#include "libCellarWorkbench_global.h"
#include "DataStructure/Vector.h"

namespace cellar
{
    template<typename T>
    class Plane3D
    {
    public:
        // Constructors
        Plane3D();
        Plane3D(T a, T b, T c, T d);
        Plane3D(const Vector<3, T>& normal, T d);
        Plane3D(const Vector<3, T>& pos,
                const Vector<3, T>& u,
                const Vector<3, T>& v);

        template<typename T2>
        Plane3D(const Plane3D<T2>& plane);


        // Getters
        T a() const;
        T b() const;
        T c() const;
        T d() const;
        Vector<3, T> normal() const;


        // Setters
        void setA(T a);
        void setB(T b);
        void setC(T c);
        void setD(T d);
        void setNormal(const Vector<3, T>& normal);
        void setPosUV(const Vector<3, T>& pos,
                      const Vector<3, T>& u,
                      const Vector<3, T>& v);


        // Computations
        T computeDistance(const Vector<3, T>& point);


    private:
        Vector<3, T> _normal;
        T           _dist;
    };
}

#endif // CELLARWORKBENCH_PLANE3D_H
