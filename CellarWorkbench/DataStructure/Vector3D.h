#ifndef CELLARWORKBENCH_VECTOR3D_H
#define CELLARWORKBENCH_VECTOR3D_H

#include <cfloat>
#include <cmath>
#include <iostream>

#include "libCellarWorkbench_global.h"
#include "Misc/CellarUtils.h"


namespace cellar
{
    template <typename T>
    class Vector3D
    {
    public :
            enum Axis {AXIS_X, AXIS_Y, AXIS_Z, AXIS_NB};

            //Constructeurs Destructeurs
            Vector3D();
            template <typename T2> Vector3D(const Vector3D<T2>& vec2);
            Vector3D(const T* array);
            Vector3D(T x, T y, T z);

            //Geters
            T x() const;
            T y() const;
            T z() const;
            T* asArray();
            const T* asArray() const;
            T distanceTo(T x=0,T y=0,T z=0) const;
            T distanceTo(const Vector3D<T>& origin) const;
            T length()const;
            T lengthSqr()const;

            //Seters
            void setX(T x);
            void setY(T y);
            void setZ(T z);

            //Operators
            Vector3D<T>   operator+ (const Vector3D<T>& vec) const;
            Vector3D<T>&  operator+=(const Vector3D<T>& vec);
            Vector3D<T>   operator- (const Vector3D<T>& vec) const;
            Vector3D<T>   operator-() const;
            Vector3D<T>&  operator-=(const Vector3D<T>& vec);
            Vector3D<T>   operator* (const T& coef)   const;
            Vector3D<T>&  operator*=(const T& coef);
            Vector3D<T>   operator/ (const T& coef)   const;
            Vector3D<T>&  operator/=(const T& coef);
            T             operator* (const Vector3D<T>& vec) const;
            Vector3D<T>   operator^ (const Vector3D<T>& vec) const;
            Vector3D<T>&  operator()(const T &x, const T &y, const T &z=0);
            T&            operator[](const int& axis);
            const T&      operator[](const int& axis) const;
            bool          operator==(const Vector3D<T>& vec) const;
            bool          operator!=(const Vector3D<T>& vec) const;
            bool          operator< (const Vector3D<T>& vec) const;
            Vector3D<T>   operator>>(const Vector3D<T>& vec) const;

            Vector3D<T>  projectionOn(const Vector3D<T>& vec) const;
            Vector3D<T>& normalize();
            Vector3D<T>  normalized() const;
            Vector3D&    rotate(const Vector3D<T>& axis, const T& angle);

    private :
            T _v[AXIS_NB];
    };

    typedef Vector3D<char> Vec3b;
    typedef Vector3D<unsigned char> Vec3ub;
    typedef Vector3D<short> Vec3s;
    typedef Vector3D<unsigned short> Vec3us;
    typedef Vector3D<int> Vec3i;
    typedef Vector3D<unsigned int> Vec3ui;
    typedef Vector3D<float> Vec3f;
    typedef Vector3D<double> Vec3d;

    template< typename T >
    std::ostream& operator<< (std::ostream& out, Vector3D<T> vec);
    template< typename T >
    std::istream& operator>> (std::istream& in, Vector3D<T>& vec);



    /* IMPLEMENTATION */

    //Constructeurs Destructeurs
    template <typename T>
    inline Vector3D<T>::Vector3D()
    {
        _v[AXIS_X] = 0;
        _v[AXIS_Y] = 0;
        _v[AXIS_Z] = 0;
    }

    template <typename T>
    template <typename T2>
    inline Vector3D<T>::Vector3D(const Vector3D<T2>& vec2)
    {
        _v[AXIS_X] = vec2[AXIS_X];
        _v[AXIS_Y] = vec2[AXIS_Y];
        _v[AXIS_Z] = vec2[AXIS_Z];
    }

    template <typename T>
    inline Vector3D<T>::Vector3D(const T* array)
    {
        _v[AXIS_X] = array[AXIS_X];
        _v[AXIS_Y] = array[AXIS_Y];
        _v[AXIS_Z] = array[AXIS_Z];
    }

    template <typename T>
    inline Vector3D<T>::Vector3D(T x, T y, T z)
    {
        _v[AXIS_X] = x;
        _v[AXIS_Y] = y;
        _v[AXIS_Z] = z;
    }

    //Geters
    template <typename T>
    inline T Vector3D<T>::x() const {return _v[AXIS_X];}

    template <typename T>
    inline T Vector3D<T>::y() const {return _v[AXIS_Y];}

    template <typename T>
    inline T Vector3D<T>::z() const {return _v[AXIS_Z];}

    template <typename T>
    inline T* Vector3D<T>::asArray()
    {
        return _v;
    }

    template <typename T>
    inline const T* Vector3D<T>::asArray() const
    {
        return _v;
    }

    template <typename T>
    inline T Vector3D<T>::distanceTo(T x,T y,T z) const
    {
            return std::sqrt((x-_v[AXIS_X])*(x-_v[AXIS_X]) +
                             (y-_v[AXIS_Y])*(y-_v[AXIS_Y]) +
                             (z-_v[AXIS_Z])*(z-_v[AXIS_Z]));
    }

    template <typename T>
    T Vector3D<T>::distanceTo(const Vector3D<T>& origin) const
    {
            return std::sqrt((origin._v[AXIS_X]-_v[AXIS_X])*(origin._v[AXIS_X]-_v[AXIS_X]) +
                             (origin._v[AXIS_Y]-_v[AXIS_Y])*(origin._v[AXIS_Y]-_v[AXIS_Y]) +
                             (origin._v[AXIS_Z]-_v[AXIS_Z])*(origin._v[AXIS_Z]-_v[AXIS_Z]));
    }

    template <typename T>
    inline T Vector3D<T>::length() const
    {
            return std::sqrt( lengthSqr() );
    }

    template <typename T>
    inline T Vector3D<T>::lengthSqr() const
    {
        return _v[AXIS_X]*_v[AXIS_X] +
               _v[AXIS_Y]*_v[AXIS_Y] +
               _v[AXIS_Z]*_v[AXIS_Z];
    }


    //Seters
    template <typename T>
    inline void Vector3D<T>::setX(T x) {_v[AXIS_X] = x;}

    template <typename T>
    inline void Vector3D<T>::setY(T y) {_v[AXIS_Y] = y;}

    template <typename T>
    inline void Vector3D<T>::setZ(T z) {_v[AXIS_Z] = z;}


    //Operateurs surcharges
    template <typename T>
    inline Vector3D<T> Vector3D<T>::operator+(const Vector3D<T>& vec) const
    {
        return Vector3D<T>(_v[AXIS_X]+vec._v[AXIS_X],
                           _v[AXIS_Y]+vec._v[AXIS_Y],
                           _v[AXIS_Z]+vec._v[AXIS_Z]);
    }

    template <typename T>
    inline Vector3D<T>& Vector3D<T>::operator+=(const Vector3D<T>& vec)
    {
        _v[AXIS_X] += vec._v[AXIS_X];
        _v[AXIS_Y] += vec._v[AXIS_Y];
        _v[AXIS_Z] += vec._v[AXIS_Z];
        return *this;
    }

    template <typename T>
    inline Vector3D<T> Vector3D<T>::operator-(const Vector3D<T>& vec) const
    {
        return Vector3D<T>(_v[AXIS_X]-vec._v[AXIS_X],
                           _v[AXIS_Y]-vec._v[AXIS_Y],
                           _v[AXIS_Z]-vec._v[AXIS_Z]);
    }

    template <typename T>
    inline Vector3D<T> Vector3D<T>::operator-() const
    {
        return Vector3D<T>(-_v[AXIS_X], -_v[AXIS_Y], -_v[AXIS_Z]);
    }

    template <typename T>
    inline Vector3D<T>& Vector3D<T>::operator-=(const Vector3D<T>& vec)
    {
        _v[AXIS_X] -= vec._v[AXIS_X];
        _v[AXIS_Y] -= vec._v[AXIS_Y];
        _v[AXIS_Z] -= vec._v[AXIS_Z];
        return *this;
    }

    template <typename T>
    inline Vector3D<T>  Vector3D<T>::operator* (const T& coef)   const
    {
        return Vector3D<T>(_v[AXIS_X]*coef,
                           _v[AXIS_Y]*coef,
                           _v[AXIS_Z]*coef);
    }

    template <typename T, typename U>
    inline Vector3D<T> operator* (const U& coef, const Vector3D<T>& vec)
    {
        return vec * coef;
    }

    template <typename T>
    inline Vector3D<T>& Vector3D<T>::operator*=(const T& coef)
    {
        _v[AXIS_X] *= coef;
        _v[AXIS_Y] *= coef;
        _v[AXIS_Z] *= coef;
        return *this;
    }

    template <typename T>
    inline Vector3D<T>  Vector3D<T>::operator/ (const T& coef)   const
    {
        return Vector3D<T>(_v[AXIS_X]/coef,
                           _v[AXIS_Y]/coef,
                           _v[AXIS_Z]/coef);
    }

    template <typename T>
    inline Vector3D<T>& Vector3D<T>::operator/=(const T& coef)
    {
        _v[AXIS_X] /= coef;
        _v[AXIS_Y] /= coef;
        _v[AXIS_Z] /= coef;
        return *this;
    }

    template <typename T>
    inline T Vector3D<T>::operator*(const Vector3D<T>& vec) const
    {
            return (vec._v[AXIS_X]*_v[AXIS_X] +
                    vec._v[AXIS_Y]*_v[AXIS_Y] +
                    vec._v[AXIS_Z]*_v[AXIS_Z]);
    }

    template <typename T>
    inline Vector3D<T> Vector3D<T>::operator^(const Vector3D<T>& vec) const
    {
        return Vector3D<T>
                (
                        _v[AXIS_Y]*vec._v[AXIS_Z] - _v[AXIS_Z]*vec._v[AXIS_Y],
                        _v[AXIS_Z]*vec._v[AXIS_X] - _v[AXIS_X]*vec._v[AXIS_Z],
                        _v[AXIS_X]*vec._v[AXIS_Y] - _v[AXIS_Y]*vec._v[AXIS_X]
                );
    }

    template <typename T>
    inline Vector3D<T>& Vector3D<T>::operator() (const T &x, const T &y, const T &z)
    {
            _v[AXIS_X] = x;
            _v[AXIS_Y] = y;
            _v[AXIS_Z] = z;
            return *this;
    }

    template <typename T>
    inline T& Vector3D<T>::operator[](const int& axis)
    {
        return
            const_cast<T&>(
                const_cast<const Vector3D<T>&>(*this)[axis]
            );
    }

    template <typename T>
    inline const T& Vector3D<T>::operator[](const int& axis) const
    {
        return _v[axis];
    }

    template <typename T>
    inline bool Vector3D<T>::operator==(const Vector3D<T>& vec) const
    {
        if( absolute(_v[AXIS_X] - vec._v[AXIS_X]) < FLT_EPSILON &&
            absolute(_v[AXIS_Y] - vec._v[AXIS_Y]) < FLT_EPSILON &&
            absolute(_v[AXIS_Z] - vec._v[AXIS_Z]) < FLT_EPSILON)
            return true;
        return false;
    }

    template <typename T>
    inline bool Vector3D<T>::operator!=(const Vector3D<T>& vec) const
    {
        return !(*this == vec);
    }

    template <typename T>
    bool Vector3D<T>::operator< (const Vector3D<T>& vec) const
    {
             if(_v[AXIS_Z] < vec._v[AXIS_Z]) return true;
        else if(_v[AXIS_Z] > vec._v[AXIS_Z]) return false;
        else if(_v[AXIS_Y] < vec._v[AXIS_Y]) return true;
        else if(_v[AXIS_Y] > vec._v[AXIS_Y]) return false;
        else if(_v[AXIS_X] < vec._v[AXIS_X]) return true;
        return false;
    }

    template <typename T>
    inline Vector3D<T> Vector3D<T>::operator>>(const Vector3D<T>& vec) const
    {
        return vec * (vec * (*this))  / vec.lengthSqr();
    }

    template <typename T>
    inline Vector3D<T> Vector3D<T>::projectionOn(const Vector3D<T>& vec) const
    {
        return (*this) >> vec;
    }

    template <typename T>
    inline Vector3D<T>& Vector3D<T>::normalize()
    {
        T n = length();
        if(n > FLT_EPSILON)
        {
            _v[AXIS_X] /= n;
            _v[AXIS_Y] /= n;
            _v[AXIS_Z] /= n;
        }
        return *this;
    }

    template <typename T>
    inline Vector3D<T> Vector3D<T>::normalized() const
    {
        return Vector3D<T>(*this).normalize();
    }

    template <typename T>
    Vector3D<T>& Vector3D<T>::rotate(const Vector3D<T>& axis, const T& angle)
    {
        T c = std::cos(angle);
        T s = std::sin(angle);
        T t = 1 - c;

        Vector3D<T> a = axis.normalized();
        T x = a.x();
        T y = a.y();
        T z = a.z();

        T vec[AXIS_NB];

        vec[AXIS_X] = (t*x*x + c)*_v[AXIS_X]   + (t*x*y - s*z)*_v[AXIS_Y] + (t*x*z + s*y)*_v[AXIS_Z];
        vec[AXIS_Y] = (t*x*y + s*z)*_v[AXIS_X] + (t*y*y + c)*_v[AXIS_Y]   + (t*y*z - s*x)*_v[AXIS_Z];
        vec[AXIS_Z] = (t*x*z - s*y)*_v[AXIS_X] + (t*y*z + s*x)*_v[AXIS_Y] + (t*z*z + c)*_v[AXIS_Z];

        _v[AXIS_X] = vec[AXIS_X];
        _v[AXIS_Y] = vec[AXIS_Y];
        _v[AXIS_Z] = vec[AXIS_Z];

        return *this;
    }

    template< typename T >
    std::ostream& operator<< (std::ostream& out, Vector3D<T> vec)
    {
        return out << '('
                   << vec.x() << ", "
                   << vec.y() << ", "
                   << vec.z()
                   << ')';
    }

    template< typename T >
    std::istream& operator>> (std::istream& in, Vector3D<T>& vec)
    {
        char c;
        return in >> c >> vec[0] >> c
                  >> c >> vec[1] >> c
                  >> c >> vec[2];
    }
}

#endif // CELLARWORKBENCH_VECTOR3D_H
