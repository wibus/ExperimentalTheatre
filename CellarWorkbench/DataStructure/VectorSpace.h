#ifndef CELLARWORKBENCH_VECTORSPACE_H
#define CELLARWORKBENCH_VECTORSPACE_H

#include "../libCellarWorkbench_global.h"
#include "Vector.h"

namespace cellar
{
    template <typename T>
    class VectorSpace
    {
    public:
        inline VectorSpace(const Vector<3, T>& origin=Vector<3, T>(),
                           const Vector<3, T>& u=Vector<3, T>(1.0, 0.0, 0.0),
                           const Vector<3, T>& v=Vector<3, T>(0.0, 1.0, 0.0),
                           const Vector<3, T>& w=Vector<3, T>(0.0, 0.0, 1.0));

        inline void makeNormalized();
        inline void makeOrthogonal();
        inline void makeOrthonormal();
        inline void rotate(const Vector<3, T>& axis, const T& angle);

        //Seters
        inline void setOrigin(const Vector<3, T>& origin);
        inline void setU(const Vector<3, T>& u);
        inline void setV(const Vector<3, T>& v);
        inline void setW(const Vector<3, T>& w);

        //Geters
        inline Vector<3, T> getOrigin() const;
        inline Vector<3, T> getU() const;
        inline Vector<3, T> getV() const;
        inline Vector<3, T> getW() const;

    private :
        Vector<3, T> _origin;
        Vector<3, T> _u;
        Vector<3, T> _v;
        Vector<3, T> _w;
    };


        // IMPLEMENTATION //
    template <typename T>
    VectorSpace<T>::VectorSpace(const Vector<3, T>& origin,
                                const Vector<3, T>& u,
                                const Vector<3, T>& v,
                                const Vector<3, T>& w)
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
    void VectorSpace<T>::rotate(const Vector<3, T>& axis, const T& angle)
    {
        _u.rotate(axis, angle);
        _v.rotate(axis, angle);
        _w.rotate(axis, angle);
    }


    //Seters
    template <typename T>
    void VectorSpace<T>::setOrigin(const Vector<3, T>& origin)
    {
        _origin = origin;
    }

    template <typename T>
    void VectorSpace<T>::setU(const Vector<3, T>& u)
    {
        _u = u;
    }

    template <typename T>
    void VectorSpace<T>::setV(const Vector<3, T>& v)
    {
        _v = v;
    }

    template <typename T>
    void VectorSpace<T>::setW(const Vector<3, T>& w)
    {
        _w = w;
    }


    //Geters
    template <typename T>
    Vector<3, T> VectorSpace<T>::getOrigin() const
    {
        return _origin;
    }

    template <typename T>
    Vector<3, T> VectorSpace<T>::getU() const
    {
        return _u;
    }

    template <typename T>
    Vector<3, T> VectorSpace<T>::getV() const
    {
        return _v;
    }

    template <typename T>
    Vector<3, T> VectorSpace<T>::getW() const
    {
        return _w;
    }
}


#endif // CELLARWORKBENCH_VECTORSPACE_H
