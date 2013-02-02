#ifndef CELLARWORKBENCH_VECTORSPACE_H
#define CELLARWORKBENCH_VECTORSPACE_H

#include "libCellarWorkbench_global.h"
#include "Vector3D.h"

namespace cellar
{
    template <typename T>
    class VectorSpace
    {
    public:
        inline VectorSpace(const Vector3D<T>& origin=Vector3D<T>(),
                           const Vector3D<T>& u=Vector3D<T>(1.0, 0.0, 0.0),
                           const Vector3D<T>& v=Vector3D<T>(0.0, 1.0, 0.0),
                           const Vector3D<T>& w=Vector3D<T>(0.0, 0.0, 1.0));

        inline void makeNormalized();
        inline void makeOrthogonal();
        inline void makeOrthonormal();
        inline void rotate(const Vector3D<T>& axis, const T& angle);

        //Seters
        inline void setOrigin(const Vector3D<T>& origin);
        inline void setU(const Vector3D<T>& u);
        inline void setV(const Vector3D<T>& v);
        inline void setW(const Vector3D<T>& w);

        //Geters
        inline Vector3D<T> getOrigin() const;
        inline Vector3D<T> getU() const;
        inline Vector3D<T> getV() const;
        inline Vector3D<T> getW() const;

    private :
        Vector3D<T> _origin;
        Vector3D<T> _u;
        Vector3D<T> _v;
        Vector3D<T> _w;
    };


        // IMPLEMENTATION //
    template <typename T>
    VectorSpace<T>::VectorSpace(const Vector3D<T>& origin,
                                const Vector3D<T>& u,
                                const Vector3D<T>& v,
                                const Vector3D<T>& w)
        : _origin(origin),
          _u(u),
          _v(v),
          _w(w)
    {
    }

    template <typename T>
    void VectorSpace<T>::makeNormalized()
    {
        _u.normalize();
        _v.normalize();
        _w.normalize();
    }

    template <typename T>
    void VectorSpace<T>::makeOrthogonal()
    {
        _w = _u ^ _v;
        _v = _w ^ _u;
    }

    template <typename T>
    void VectorSpace<T>::makeOrthonormal()
    {
        makeOrthogonal();
        makeNormalized();
    }

    template <typename T>
    void VectorSpace<T>::rotate(const Vector3D<T>& axis, const T& angle)
    {
        _u.rotate(axis, angle);
        _v.rotate(axis, angle);
        _w.rotate(axis, angle);
    }


    //Seters
    template <typename T>
    void VectorSpace<T>::setOrigin(const Vector3D<T>& origin)
    {
        _origin = origin;
    }

    template <typename T>
    void VectorSpace<T>::setU(const Vector3D<T>& u)
    {
        _u = u;
    }

    template <typename T>
    void VectorSpace<T>::setV(const Vector3D<T>& v)
    {
        _v = v;
    }

    template <typename T>
    void VectorSpace<T>::setW(const Vector3D<T>& w)
    {
        _w = w;
    }


    //Geters
    template <typename T>
    Vector3D<T> VectorSpace<T>::getOrigin() const
    {
        return _origin;
    }

    template <typename T>
    Vector3D<T> VectorSpace<T>::getU() const
    {
        return _u;
    }

    template <typename T>
    Vector3D<T> VectorSpace<T>::getV() const
    {
        return _v;
    }

    template <typename T>
    Vector3D<T> VectorSpace<T>::getW() const
    {
        return _w;
    }
}


#endif // CELLARWORKBENCH_VECTORSPACE_H
