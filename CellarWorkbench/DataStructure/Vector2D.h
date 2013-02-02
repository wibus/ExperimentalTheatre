#ifndef CELLARWORKBENCH_VECTOR2D_H
#define CELLARWORKBENCH_VECTOR2D_H

#include <cfloat>
#include <cmath>
#include <iostream>

#include "libCellarWorkbench_global.h"
#include "Misc/CellarUtils.h"

namespace cellar
{
    template <typename T>
    class Vector2D
    {
    public :
            enum Axis {AXIS_X, AXIS_Y, AXIS_NB};

            //Constructeurs Destructeurs
            Vector2D();
            template <typename T2> Vector2D(const Vector2D<T2>& vec2);
            Vector2D(const T* array);
            Vector2D(T x, T y);

            //Geters
            T x() const;
            T y() const;
            T* asArray();
            const T* asArray() const;
            T distanceTo(const T& x=0,const T& y=0) const;
            T distanceTo(const Vector2D<T>& origin) const;
            T length() const;
            T lengthSqr() const;

            //Seters
            void setX(T x);
            void setY(T y);

            //Operators
            Vector2D<T>  operator+ (const Vector2D<T>& vec) const;
            Vector2D<T>& operator+=(const Vector2D<T>& vec);
            Vector2D<T>  operator- (const Vector2D<T>& vec) const;
            Vector2D<T>  operator- () const;
            Vector2D<T>& operator-=(const Vector2D<T>& vec);
            Vector2D<T>  operator* (const T& coef) const;
            Vector2D<T>& operator*=(const T& coef);
            Vector2D<T>  operator/ (const T& coef) const;
            Vector2D<T>& operator/=(const T& coef);
            T            operator* (const Vector2D<T>& vec) const;
            T            operator^ (const Vector2D<T>& vec) const;
            Vector2D<T>& operator()(const T &x, const T &y);
            T&           operator[](const int& axis);
            const T&     operator[](const int& axis) const;
            bool         operator==(const Vector2D<T>& vec) const;
            bool         operator!=(const Vector2D<T>& vec) const;
            bool         operator< (const Vector2D<T>& vec) const;
            Vector2D<T>  operator>>(const Vector2D<T>& vec) const;

            Vector2D<T>  projectionOn(const Vector2D<T>& vec) const;
            Vector2D<T>& normalize();
            Vector2D<T>  normalized() const;
            Vector2D<T>& rotate(const T& angle);
            Vector2D<T>& rotateQuarterCW();
            Vector2D<T>& rotateQuarterCCW();

    private :
            T _v[AXIS_NB];            
    };

    typedef Vector2D<char> Vec2b;
    typedef Vector2D<unsigned char> Vec2ub;
    typedef Vector2D<short> Vec2s;
    typedef Vector2D<unsigned short> Vec2us;
    typedef Vector2D<int> Vec2i;
    typedef Vector2D<unsigned int> Vec2ui;
    typedef Vector2D<float> Vec2f;
    typedef Vector2D<double> Vec2d;

    template< typename T >
    std::ostream& operator<< (std::ostream& out, const Vector2D<T>& vec);
    template< typename T >
    std::istream& operator>> (std::istream& in, Vector2D<T>& vec);




    // IMPLEMENTATION //

    //Constructors
    template <typename T>
    inline Vector2D<T>::Vector2D()
    {
        _v[AXIS_X] = 0;
        _v[AXIS_Y] = 0;
    }

    template <typename T>
    template <typename T2>
    inline Vector2D<T>::Vector2D(const Vector2D<T2>& vec2)
    {
        _v[AXIS_X] = vec2[AXIS_X];
        _v[AXIS_Y] = vec2[AXIS_Y];
    }

    template <typename T>
    inline Vector2D<T>::Vector2D(const T* array)
    {
        _v[AXIS_X] = array[AXIS_X];
        _v[AXIS_Y] = array[AXIS_Y];
    }

    template <typename T>
    inline Vector2D<T>::Vector2D(T x, T y)
    {
        _v[AXIS_X] = x;
        _v[AXIS_Y] = y;
    }


    // Geters
    template <typename T>
    inline T Vector2D<T>::x() const
        {return _v[AXIS_X];}

    template <typename T>
    inline T Vector2D<T>::y() const
        {return _v[AXIS_Y];}

    template <typename T>
    inline T* Vector2D<T>::asArray()
        {return _v;}

    template <typename T>
    inline const T* Vector2D<T>::asArray() const
    {
        return _v;
    }

    template <typename T>
    inline T Vector2D<T>::distanceTo(const T& x,const T& y) const
    {
        return std::sqrt((x-_v[AXIS_X])*(x-_v[AXIS_X]) +
                         (y-_v[AXIS_Y])*(y-_v[AXIS_Y]));
    }

    template <typename T>
    inline T Vector2D<T>::distanceTo(const Vector2D<T>& origin) const
    {
        return std::sqrt((origin._v[AXIS_X]-_v[AXIS_X])*(origin._v[AXIS_X]-_v[AXIS_X]) +
                         (origin._v[AXIS_Y]-_v[AXIS_Y])*(origin._v[AXIS_Y]-_v[AXIS_Y]));
    }

    template <typename T>
    inline T Vector2D<T>::length() const
    {
        return std::sqrt( lengthSqr() );
    }

    template <typename T>
    inline T Vector2D<T>::lengthSqr() const
    {
        return _v[AXIS_X]*_v[AXIS_X] +
               _v[AXIS_Y]*_v[AXIS_Y];
    }



    // Seters
    template <typename T>
    inline void Vector2D<T>::setX(T x) {_v[AXIS_X] = x;}

    template <typename T>
    inline void Vector2D<T>::setY(T y) {_v[AXIS_Y] = y;}




    //Operateurs surcharges
    template <typename T>
    inline Vector2D<T> Vector2D<T>::operator+(const Vector2D<T>& vec) const
    {
        return Vector2D<T>(_v[AXIS_X] + vec._v[AXIS_X], _v[AXIS_Y] + vec._v[AXIS_Y]);
    }

    template <typename T>
    inline Vector2D<T>& Vector2D<T>::operator+=(const Vector2D<T>& vec)
    {
        _v[AXIS_X] += vec._v[AXIS_X];
        _v[AXIS_Y] += vec._v[AXIS_Y];
        return *this;
    }

    template <typename T>
    inline Vector2D<T> Vector2D<T>::operator-(const Vector2D<T>& vec) const
    {
        return Vector2D<T>(_v[AXIS_X] - vec._v[AXIS_X],
                           _v[AXIS_Y] - vec._v[AXIS_Y]);
    }

    template <typename T>
    inline Vector2D<T> Vector2D<T>::operator-() const
    {
        return Vector2D<T>(-_v[AXIS_X],
                           -_v[AXIS_Y]);
    }

    template <typename T>
    inline Vector2D<T>& Vector2D<T>::operator-=(const Vector2D<T>& vec)
    {
        _v[AXIS_X] -= vec._v[AXIS_X];
        _v[AXIS_Y] -= vec._v[AXIS_Y];
        return *this;
    }

    template <typename T>
    inline Vector2D<T>  Vector2D<T>::operator* (const T& coef) const
    {
        return Vector2D<T>(_v[AXIS_X]*coef,
                           _v[AXIS_Y]*coef);
    }

    template <typename T, typename U>
    inline Vector2D<T> operator* (const U& coef, const Vector2D<T>& vec)
    {
        return vec * coef;
    }

    template <typename T>
    inline Vector2D<T>& Vector2D<T>::operator*=(const T& coef)
    {
        _v[AXIS_X] *= coef;
        _v[AXIS_Y] *= coef;
        return *this;
    }

    template <typename T>
    inline Vector2D<T>  Vector2D<T>::operator/ (const T& coef) const
    {
        return Vector2D<T>(_v[AXIS_X] / coef,
                           _v[AXIS_Y] / coef);
    }

    template <typename T>
    inline Vector2D<T>& Vector2D<T>::operator/=(const T& coef)
    {
        _v[AXIS_X] /= coef;
        _v[AXIS_Y] /= coef;
        return *this;
    }

    template <typename T>
    inline T Vector2D<T>::operator* (const Vector2D<T>& vec) const
    {
        return _v[AXIS_X] * vec._v[AXIS_X] +
               _v[AXIS_Y] * vec._v[AXIS_Y];
    }

    template <typename T>
    inline T Vector2D<T>::operator^(const Vector2D<T>& vec) const
    {
        return (_v[AXIS_X] * vec._v[AXIS_Y]) -
               (_v[AXIS_Y] * vec._v[AXIS_X]);
    }

    template <typename T>
    inline Vector2D<T>& Vector2D<T>::operator() (const T& x, const T& y)
    {
            _v[AXIS_X] = x;
            _v[AXIS_Y] = y;
            return *this;
    }

    template <typename T>
    inline T& Vector2D<T>::operator[](const int& axis)
    {
        return
            const_cast<T&>(
                const_cast<const Vector2D<T>&>(*this)[axis]
            );
    }

    template <typename T>
    inline const T& Vector2D<T>::operator[](const int& axis) const
    {
        return _v[axis];
    }

    template <typename T>
    inline bool Vector2D<T>::operator==(const Vector2D<T>& vec) const
    {
        if( absolute(_v[AXIS_X] - vec._v[AXIS_X]) < FLT_EPSILON &&
            absolute(_v[AXIS_Y] - vec._v[AXIS_Y]) < FLT_EPSILON)
            return true;
        return false;
    }

    template <typename T>
    inline bool Vector2D<T>::operator!=(const Vector2D<T>& vec) const
    {
        return !(*this == vec);
    }

    template <typename T>
    bool Vector2D<T>::operator< (const Vector2D<T>& vec) const
    {
             if(_v[AXIS_Y] < vec._v[AXIS_Y]) return true;
        else if(_v[AXIS_Y] > vec._v[AXIS_Y]) return false;
        else if(_v[AXIS_X] < vec._v[AXIS_X]) return true;
        return false;
    }

    template <typename T>
    inline Vector2D<T> Vector2D<T>::operator>>(const Vector2D<T>& vec) const
    {
        return vec * (vec * (*this))  / vec.lengthSqr();
    }

    template <typename T>
    inline Vector2D<T> Vector2D<T>::projectionOn(const Vector2D<T>& vec) const
    {
        return (*this) >> vec;
    }

    template <typename T>
    inline Vector2D<T>& Vector2D<T>::normalize()
    {
        T n = length();
        if(n > FLT_EPSILON)
        {
            _v[AXIS_X] /= n;
            _v[AXIS_Y] /= n;
        }
        return *this;
    }

    template <typename T>
    inline Vector2D<T> Vector2D<T>::normalized() const
    {
        return Vector2D<T>(*this).normalize();
    }

    template <typename T>
    Vector2D<T>& Vector2D<T>::rotate(const T& angle)
    {
        T x = cos(angle)*_v[AXIS_X] - sin(angle)*_v[AXIS_Y];
        T y = sin(angle)*_v[AXIS_X] + cos(angle)*_v[AXIS_Y];
        _v[AXIS_X] = x; _v[AXIS_Y] = y;
        return *this;
    }

    template <typename T>
    Vector2D<T>& Vector2D<T>::rotateQuarterCW()
    {
        T temp = -_v[AXIS_X];
        _v[AXIS_X] = _v[AXIS_Y];
        _v[AXIS_Y] = temp;
        return *this;
    }

    template <typename T>
    Vector2D<T>& Vector2D<T>::rotateQuarterCCW()
    {
        T temp = -_v[AXIS_Y];
        _v[AXIS_Y] = _v[AXIS_X];
        _v[AXIS_X] = temp;
        return *this;
    }

    template< typename T >
    std::ostream& operator<< (std::ostream& out, const Vector2D<T>& vec)
    {
        return out << '('
                   << vec.x() << ", "
                   << vec.y()
                   << ')';
    }

    template< typename T >
    std::istream& operator>> (std::istream& in, Vector2D<T>& vec)
    {
        char c;
        return in >> c >> vec[0] >> c
                  >> c >> vec[1];
    }
}

#endif // CELLARWORKBENCH_VECTOR2D_H
