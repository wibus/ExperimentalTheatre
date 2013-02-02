#ifndef CELLARWORKBENCH_VECTOR4D_H
#define CELLARWORKBENCH_VECTOR4D_H

#include <cfloat>
#include <cmath>
#include <iostream>

#include "libCellarWorkbench_global.h"
#include "Misc/CellarUtils.h"


namespace cellar
{
    template <typename T>
    class Vector4D
    {
    public :
            enum Axis {AXIS_X, AXIS_Y, AXIS_Z, AXIS_W, AXIS_NB};

            //Constructeurs Destructeurs
            Vector4D();
            template <typename T2> Vector4D(const Vector4D<T2>& vec2);
            Vector4D(const T* array);
            Vector4D(T x, T y, T z, T w=1);

            //Geters
            T x() const;
            T y() const;
            T z() const;
            T w() const;
            T* asArray();
            const T* asArray() const;
            T distanceTo(T x=0,T y=0,T z=0, T w=1) const;
            T distanceTo(const Vector4D<T>& origin) const;
            T length()const;
            T lengthSqr() const;

            //Seters
            void setX(T x);
            void setY(T y);
            void setZ(T z);
            void setW(T w);

            //Operators
            Vector4D<T>   operator+ (const Vector4D<T>& vec) const;
            Vector4D<T>&  operator+=(const Vector4D<T>& vec);
            Vector4D<T>   operator- (const Vector4D<T>& vec) const;
            Vector4D<T>   operator-() const;
            Vector4D<T>&  operator-=(const Vector4D<T>& vec);
            Vector4D<T>   operator* (const T& coef)   const;
            Vector4D<T>&  operator*=(const T& coef);
            Vector4D<T>   operator/ (const T& coef)   const;
            Vector4D<T>&  operator/=(const T& coef);
            T             operator* (const Vector4D<T>& vec) const;
            Vector4D<T>&  operator()(const T x, const T y, const T z, const T w=1);
            T&            operator[](const int& axis);
            const T&      operator[](const int& axis) const;
            bool          operator==(const Vector4D<T>& vec) const;
            bool          operator!=(const Vector4D<T>& vec) const;
            bool          operator< (const Vector4D<T>& vec) const;
            Vector4D<T>   operator>>(const Vector4D<T>& vec) const;

            Vector4D<T>  projectionOn(const Vector4D<T>& vec) const;
            Vector4D<T>& normalize();
            Vector4D<T>  normalized() const;

    private :
            T _v[AXIS_NB];
    };

    typedef Vector4D<char> Vec4b;
    typedef Vector4D<unsigned char> Vec4ub;
    typedef Vector4D<short> Vec4s;
    typedef Vector4D<unsigned short> Vec4us;
    typedef Vector4D<int> Vec4i;
    typedef Vector4D<unsigned int> Vec4ui;
    typedef Vector4D<float> Vec4f;
    typedef Vector4D<double> Vec4d;

    template< typename T >
    std::ostream& operator<< (std::ostream& out, Vector4D<T> vec);
    template< typename T >
    std::istream& operator>> (std::istream& in, Vector4D<T>& vec);


    /* IMPLEMENTATION */

    //Constructeurs Destructeurs
    template <typename T>
    inline Vector4D<T>::Vector4D()
    {
        _v[AXIS_X] = 0;
        _v[AXIS_Y] = 0;
        _v[AXIS_Z] = 0;
        _v[AXIS_W] = 1;
    }

    template <typename T>
    template <typename T2>
    inline Vector4D<T>::Vector4D(const Vector4D<T2>& vec2)
    {
        _v[AXIS_X] = vec2[AXIS_X];
        _v[AXIS_Y] = vec2[AXIS_Y];
        _v[AXIS_Z] = vec2[AXIS_Z];
        _v[AXIS_W] = vec2[AXIS_W];
    }

    template <typename T>
    inline Vector4D<T>::Vector4D(const T* array)
    {
        _v[AXIS_X] = array[AXIS_X];
        _v[AXIS_Y] = array[AXIS_Y];
        _v[AXIS_Z] = array[AXIS_Z];
        _v[AXIS_W] = array[AXIS_W];
    }

    template <typename T>
    inline Vector4D<T>::Vector4D(T x, T y, T z, T w)
    {
        _v[AXIS_X] = x;
        _v[AXIS_Y] = y;
        _v[AXIS_Z] = z;
        _v[AXIS_W] = w;
    }


    //Geters
    template <typename T>
    inline T Vector4D<T>::x() const {return _v[AXIS_X];}

    template <typename T>
    inline T Vector4D<T>::y() const {return _v[AXIS_Y];}

    template <typename T>
    inline T Vector4D<T>::z() const {return _v[AXIS_Z];}

    template <typename T>
    inline T Vector4D<T>::w() const {return _v[AXIS_W];}

    template <typename T>
    inline T* Vector4D<T>::asArray()
    {
        return _v;
    }

    template <typename T>
    inline const T* Vector4D<T>::asArray() const
    {
        return _v;
    }

    template <typename T>
    inline T Vector4D<T>::distanceTo(T x, T y, T z, T w) const
    {
            return std::sqrt((x-_v[AXIS_X])*(x-_v[AXIS_X]) +
                             (y-_v[AXIS_Y])*(y-_v[AXIS_Y]) +
                             (z-_v[AXIS_Z])*(z-_v[AXIS_Z]) +
                             (w-_v[AXIS_W])*(w-_v[AXIS_W]));
    }

    template <typename T>
    inline T Vector4D<T>::distanceTo(const Vector4D<T>& origin) const
    {
            return std::sqrt((origin._v[AXIS_X]-_v[AXIS_X])*(origin._v[AXIS_X]-_v[AXIS_X]) +
                             (origin._v[AXIS_Y]-_v[AXIS_Y])*(origin._v[AXIS_Y]-_v[AXIS_Y]) +
                             (origin._v[AXIS_Z]-_v[AXIS_Z])*(origin._v[AXIS_Z]-_v[AXIS_Z]) +
                             (origin._v[AXIS_W]-_v[AXIS_W])*(origin._v[AXIS_W]-_v[AXIS_W]));
    }

    template <typename T>
    inline T Vector4D<T>::length() const
    {
            return std::sqrt( lengthSqr() );
    }

    template <typename T>
    inline T Vector4D<T>::lengthSqr() const
    {
        return _v[AXIS_X]*_v[AXIS_X] +
               _v[AXIS_Y]*_v[AXIS_Y] +
               _v[AXIS_Z]*_v[AXIS_Z] +
               _v[AXIS_W]*_v[AXIS_W];
    }


    //Seters
    template <typename T>
    inline void Vector4D<T>::setX(T x) {_v[AXIS_X] = x;}

    template <typename T>
    inline void Vector4D<T>::setY(T y) {_v[AXIS_Y] = y;}

    template <typename T>
    inline void Vector4D<T>::setZ(T z) {_v[AXIS_Z] = z;}

    template <typename T>
    inline void Vector4D<T>::setW(T w) {_v[AXIS_W] = w;}


    //Operateurs surcharges
    template <typename T>
    inline Vector4D<T> Vector4D<T>::operator+(const Vector4D<T>& vec) const
    {
        return Vector4D<T>(_v[AXIS_X]+vec._v[AXIS_X],
                           _v[AXIS_Y]+vec._v[AXIS_Y],
                           _v[AXIS_Z]+vec._v[AXIS_Z],
                           _v[AXIS_W]+vec._v[AXIS_W]);
    }

    template <typename T>
    inline Vector4D<T>& Vector4D<T>::operator+=(const Vector4D<T>& vec)
    {
        _v[AXIS_X] += vec._v[AXIS_X];
        _v[AXIS_Y] += vec._v[AXIS_Y];
        _v[AXIS_Z] += vec._v[AXIS_Z];
        _v[AXIS_W] += vec._v[AXIS_W];
        return *this;
    }

    template <typename T>
    inline Vector4D<T> Vector4D<T>::operator-(const Vector4D<T>& vec) const
    {
        return Vector4D<T>(_v[AXIS_X]-vec._v[AXIS_X],
                           _v[AXIS_Y]-vec._v[AXIS_Y],
                           _v[AXIS_Z]-vec._v[AXIS_Z],
                           _v[AXIS_W]-vec._v[AXIS_W]);
    }

    template <typename T>
    inline Vector4D<T> Vector4D<T>::operator-() const{
        return Vector4D<T>(-_v[AXIS_X],
                           -_v[AXIS_Y],
                           -_v[AXIS_Z],
                           -_v[AXIS_W]);
    }

    template <typename T>
    inline Vector4D<T>& Vector4D<T>::operator-=(const Vector4D<T>& vec)
    {
        _v[AXIS_X] -= vec._v[AXIS_X];
        _v[AXIS_Y] -= vec._v[AXIS_Y];
        _v[AXIS_Z] -= vec._v[AXIS_Z];
        _v[AXIS_W] -= vec._v[AXIS_W];
        return *this;
    }

    template <typename T>
    inline Vector4D<T>  Vector4D<T>::operator* (const T& coef)   const
    {
        return Vector4D<T>(_v[AXIS_X]*coef,
                           _v[AXIS_Y]*coef,
                           _v[AXIS_Z]*coef,
                           _v[AXIS_W]*coef);
    }

    template <typename T, typename U>
    inline Vector4D<T> operator* (const U& coef, const Vector4D<T>& vec)
    {
        return vec * coef;
    }

    template <typename T>
    inline Vector4D<T>& Vector4D<T>::operator*=(const T& coef)
    {
        _v[AXIS_X] *= coef;
        _v[AXIS_Y] *= coef;
        _v[AXIS_Z] *= coef;
        _v[AXIS_W] *= coef;
        return *this;
    }

    template <typename T>
    inline Vector4D<T>  Vector4D<T>::operator/ (const T& coef)   const
    {
        return Vector4D<T>(_v[AXIS_X]/coef,
                           _v[AXIS_Y]/coef,
                           _v[AXIS_Z]/coef,
                           _v[AXIS_W]/coef);
    }

    template <typename T>
    inline Vector4D<T>& Vector4D<T>::operator/=(const T& coef)
    {
        _v[AXIS_X] /= coef;
        _v[AXIS_Y] /= coef;
        _v[AXIS_Z] /= coef;
        _v[AXIS_W] /= coef;
        return *this;
    }

    template <typename T>
    inline T Vector4D<T>::operator*(const Vector4D<T>& vec) const
    {
            return (vec._v[AXIS_X]*_v[AXIS_X] +
                    vec._v[AXIS_Y]*_v[AXIS_Y] +
                    vec._v[AXIS_Z]*_v[AXIS_Z] +
                    vec._v[AXIS_W]*_v[AXIS_W]);
    }

    template <typename T>
    inline Vector4D<T>& Vector4D<T>::operator() (const T x, const T y, const T z, const T w)
    {
            _v[AXIS_X] = x;
            _v[AXIS_Y] = y;
            _v[AXIS_Z] = z;
            _v[AXIS_W] = w;
            return *this;
    }

    template <typename T>
    inline T& Vector4D<T>::operator[](const int& axis)
    {
        return
            const_cast<T&>(
                const_cast<const Vector4D<T>&>(*this)[axis]
            );
    }

    template <typename T>
    inline const T& Vector4D<T>::operator[](const int& axis) const
    {
        return _v[axis];
    }

    template <typename T>
    inline bool Vector4D<T>::operator==(const Vector4D<T>& vec) const
    {
        if( absolute(_v[AXIS_X] - vec._v[AXIS_X]) < FLT_EPSILON &&
            absolute(_v[AXIS_Y] - vec._v[AXIS_Y]) < FLT_EPSILON &&
            absolute(_v[AXIS_Z] - vec._v[AXIS_Z]) < FLT_EPSILON &&
            absolute(_v[AXIS_W] - vec._v[AXIS_W]) < FLT_EPSILON)
            return true;
        return false;
    }

    template <typename T>
    bool Vector4D<T>::operator!=(const Vector4D<T>& vec) const
    {
        return !(*this == vec);
    }

    template <typename T>
    bool Vector4D<T>::operator< (const Vector4D<T>& vec) const
    {
             if(_v[AXIS_W] < vec._v[AXIS_W]) return true;
        else if(_v[AXIS_W] > vec._v[AXIS_W]) return false;
        else if(_v[AXIS_Z] < vec._v[AXIS_Z]) return true;
        else if(_v[AXIS_Z] > vec._v[AXIS_Z]) return false;
        else if(_v[AXIS_Y] < vec._v[AXIS_Y]) return true;
        else if(_v[AXIS_Y] > vec._v[AXIS_Y]) return false;
        else if(_v[AXIS_X] < vec._v[AXIS_X]) return true;
        return false;
    }

    template <typename T>
    inline Vector4D<T> Vector4D<T>::operator>>(const Vector4D<T>& vec) const
    {
        return vec * (vec * (*this)) / vec.lengthSqr();
    }

    template <typename T>
    inline Vector4D<T> Vector4D<T>::projectionOn(const Vector4D<T>& vec) const
    {
        return (*this) >> vec;
    }

    template <typename T>
    inline Vector4D<T>& Vector4D<T>::normalize()
    {
        T n = length();
        if(n > FLT_EPSILON)
        {
            _v[AXIS_X] /= n;
            _v[AXIS_Y] /= n;
            _v[AXIS_Z] /= n;
            _v[AXIS_W] /= n;
        }
        return *this;
    }

    template <typename T>
    inline Vector4D<T> Vector4D<T>::normalized() const
    {
        return Vector4D<T>(*this).normalize();
    }

    template< typename T >
    std::ostream& operator<< (std::ostream& out, Vector4D<T> vec)
    {
        return out << '('
                   << vec.x() << ", "
                   << vec.y() << ", "
                   << vec.z() << ", "
                   << vec.w()
                   << ')';
    }

    template< typename T >
    std::istream& operator>> (std::istream& in, Vector4D<T>& vec)
    {
        char c;
        return in >> c >> vec[0] >> c
                  >> c >> vec[1] >> c
                  >> c >> vec[2] >> c
                  >> c >> vec[3];
    }
}


#endif // CELLARWORKBENCH_VECTOR4D_H
