#ifndef CELLARWORKBENCH_LINE3D_H
#define CELLARWORKBENCH_LINE3D_H

#include "libCellarWorkbench_global.h"
#include "DataStructure/Vector3D.h"

namespace cellar
{
    template<typename T>
    class Segment3D
    {
    public:
        Segment3D();
        Segment3D(const Vector3D<T>& begin, const Vector3D<T>& end);
        Segment3D(const Vector3D<T>& begin, const Vector3D<T>& length);

        template<typename T2>
        Segment3D(const Segment3D<T2>& line);


        Vector3D<T> begin() const;
        Vector3D<T> end() const;
        Vector3D<T> length() const;


        void setBegin(const Vector3D<T>& begin);
        void setEnd(const Vector3D<T>& end);
        void setLength(const Vector3D<T>& length);


    private:
        Vector3D<T> _begin;
        Vector3D<T> _end;
    };
}


#endif // CELLARWORKBENCH_LINE3D_H
