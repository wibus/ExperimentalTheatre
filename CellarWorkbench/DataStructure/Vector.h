#ifndef CELLARWORKBENCH_VECTOR_H
#define CELLARWORKBENCH_VECTOR_H

#include <cfloat>
#include <cmath>

#include <iostream>

#include "../libCellarWorkbench_global.h"


namespace cellar
{

/// Algebraic vector of any dimension. An array of Vector<N,T> can be treated as
/// an array of T, where T is the type of the components of the Vector<N,T>.
/// This is really useful feature for working with C libraries like Openg GL.
template <int N, typename T>
class Vector
{
public :
    /// Constructor
    /// \param[in] array Array containing the components of the vector
    Vector(const T* array);

    /// Constructor
    /// \param[in] x First component of the vector
    /// \param[in] y Second component of the vector
    /// \param[in] z Third component of the vector
    /// \param[in] w Fourth component of the vector
    /// \note Also the default constructor
    explicit Vector(const T& x=T(0),
                    const T& y=T(0),
                    const T& z=T(0),
                    const T& w=T(1));

    /// Copy constructor for vector of different size or component type
    /// \param[in] vec Vector to copy
    template <int N2, typename T2>
    Vector(const Vector<N2, T2>& vec);

    /// Copy constructor for 2D vectors with additional specified components
    /// \param[in] vec Vector to copy
    /// \param[in] z Third component of the vector
    /// \param[in] w Fourth component of the vector
    template <typename T2>
    Vector(const Vector<2, T2>& vec,
           T z,
           T w=T(1));

    /// Copy constructor for 3D vectors with an additional specified component
    /// \param[in] vec Vector to copy
    /// \param[in] w Fourth component of the vector
    template <typename T2>
    Vector(const Vector<3, T2>& vec,
           T w);

    /// Returns the number of components of the vector
    int getNbComponents() const;

    /// Returns the first component of the vector
    /// \return First coponent of the vector
    T x() const;

    /// Returns the second component of the vector
    /// \return Second coponent of the vector
    T y() const;

    /// Returns the third component of the vector
    /// \return Third coponent of the vector if N>2, zero otherwise
    T z() const;

    /// Returns the fourth component of the vector
    /// \return Fourth coponent of the vector if N>3, one otherwise
    T w() const;

    /// Returns a pointer to the internal coponents array
    /// \return Pointer to components array
    T* asArray();

    /// Returns a pointer to the internal coponents array
    /// \return Const pointer to components array
    const T* asArray() const;

    /// Computes the distance between this and the given vector
    /// \param[in] vec The vector to compute the distance with
    /// \return The distance between this and the given vector
    /// \note Expressed in the same type as the components
    T distanceTo(const Vector<N, T>& vec) const;

    /// Computes the length of the vector
    /// \return The length of this vector
    /// \note Expressed in the same type as the components
    T length() const;

    /// Computes the square of the length of the vector
    /// \return The square of the length of this vector
    /// \note Expressed in the same type as the components
    T length2() const;


    Vector<N, T>& normalize();
    Vector<N, T>  normalized() const;
    T angle() const;

    //Seters
    void setX(T x);
    void setY(T y);
    void setZ(T z);
    void setW(T w);

    //Operators
    Vector<N, T>   operator+ (const Vector<N, T>& vec) const;
    Vector<N, T>&  operator+=(const Vector<N, T>& vec);
    Vector<N, T>   operator- () const;
    Vector<N, T>   operator- (const Vector<N, T>& vec) const;
    Vector<N, T>&  operator-=(const Vector<N, T>& vec);
    Vector<N, T>   operator* (const T& coef)   const;
    Vector<N, T>&  operator*=(const T& coef);
    Vector<N, T>   operator/ (const T& coef)   const;
    Vector<N, T>&  operator/=(const T& coef);
    Vector<N, T>&  operator()(const T& x, const T& y, const T& z=0, const T& w=1);
    T&             operator[](int axis);
    const T&       operator[](int axis) const;
    bool           operator==(const Vector<N, T>& vec) const;
    bool           operator!=(const Vector<N, T>& vec) const;
    bool           operator< (const Vector<N, T>& vec) const;
    operator bool() const;

    static Vector<2, T> polar(const T& angle, const T& radius = T(1));

private :
    T _v[N];
};

typedef Vector<2, unsigned char> Vec2ub;
typedef Vector<2, int> Vec2i;
typedef Vector<2, float> Vec2f;
typedef Vector<2, double> Vec2d;

typedef Vector<3, unsigned char> Vec3ub;
typedef Vector<3, int> Vec3i;
typedef Vector<3, float> Vec3f;
typedef Vector<3, double> Vec3d;

typedef Vector<4, unsigned char> Vec4ub;
typedef Vector<4, int> Vec4i;
typedef Vector<4, float> Vec4f;
typedef Vector<4, double> Vec4d;

template<int N, typename T>
Vector<N,T> multiply(const Vector<N,T>& vec1, const Vector<N,T>& vec2);


template<int N, typename T>
Vector<N,T> divide(const Vector<N,T>& vec1, const Vector<N,T>& vec2);


template<int N, typename T>
T dot(const Vector<N,T>& vec1, const Vector<N,T>& vec2);


template<typename T>
T cross(const Vector<2,T>& vec1, const Vector<2,T>& vec2);


template<typename T>
Vector<3,T> cross(const Vector<3,T>& vec1, const Vector<3,T>& vec2);


template<int N, typename T >
Vector<N, T> proj(const Vector<N, T>& vec, const Vector<N, T>& unitVec);


template<typename T>
Vector<2,T> perpCW(const Vector<2,T>& vec);


template<typename T>
Vector<2,T> perpCCW(const Vector<2,T>& vec);


template<typename T>
Vector<2,T> rotate(const Vector<2,T>& vec, double radians);


template<int N, typename T >
std::ostream& operator<< (std::ostream& out, const Vector<N, T>& vec);


template<int N, typename T >
std::istream& operator>> (std::istream& in, Vector<N, T>& vec);



// IMPLEMENTATION //
template <int N, typename T>
inline Vector<N, T>::Vector(const T* array)
{
    for(int i=0; i<N; ++i)
        _v[i] = array[i];
}

template <int N, typename T>
inline Vector<N, T>::Vector(const T& x, const T& y, const T& z, const T& w)
{
    _v[0] = x;
    _v[1] = y;
    if(2 < N) _v[2] = z;
    if(3 < N) _v[3] = w;
}

template <int N, typename T>
template <int N2, typename T2>
Vector<N, T>::Vector(const Vector<N2, T2>& vec)
{
    if( N == N2 || N < N2)
    {
        for(int i=0; i<N; ++i)
            _v[i] = vec[i];
    }
    else
    {
        for(int i=0; i<N2; ++i)
            _v[i] = vec[i];
        for(int i=N2; i<N; ++i)
            _v[i] = T(0);
        if(3 < N) _v[3] = T(1);
    }
}

template <int N, typename T>
template <typename T2>
inline Vector<N, T>::Vector(const Vector<2, T2>& vec, T z, T w)
{
    _v[0] = vec[0];
    _v[1] = vec[1];
    if(2 < N) _v[2] = z;
    if(3 < N) _v[3] = w;
}

template <int N, typename T>
template <typename T2>
inline Vector<N, T>::Vector(const Vector<3, T2>& vec, T w)
{
    _v[0] = vec[0];
    _v[1] = vec[1];
    if(2 < N) _v[2] = vec[2];
    if(3 < N) _v[3] = w;
}


//Geters
template <int N, typename T>
inline int Vector<N, T>::getNbComponents() const {return N;}

template <int N, typename T>
inline T Vector<N, T>::x() const {return _v[0];}

template <int N, typename T>
inline T Vector<N, T>::y() const {return _v[1];}

template <int N, typename T>
inline T Vector<N, T>::z() const {return N < 3 ? T(0) : _v[2];}

template <int N, typename T>
inline T Vector<N, T>::w() const {return N < 4 ? T(1) : _v[3];}

template <int N, typename T>
inline T* Vector<N, T>::asArray()
{
    return _v;
}

template <int N, typename T>
inline const T* Vector<N, T>::asArray() const
{
    return _v;
}

template <int N, typename T>
inline T Vector<N, T>::distanceTo(const Vector<N, T>& vec) const
{
    return ((*this) - vec).length();
}

template <int N, typename T>
inline T Vector<N, T>::length() const
{
    return std::sqrt( length2() );
}

template <int N, typename T>
inline T Vector<N, T>::length2() const
{
    return dot(*this, *this);
}

template <int N, typename T>
Vector<N, T>& Vector<N, T>::normalize()
{
    T n = static_cast<T>(length());
    if(n > FLT_EPSILON)
    {
        for(int i=0; i<N; ++i)
            _v[i] /= n;
    }
    return *this;
}

template <int N, typename T>
inline Vector<N, T> Vector<N, T>::normalized() const
{
    return Vector<N, T>(*this).normalize();
}

template <int N, typename T>
inline T Vector<N, T>::angle() const
{
    return ::atan2(_v[1], _v[0]);
}


//Seters
template <int N, typename T>
inline void Vector<N, T>::setX(T x) {_v[0] = x;}

template <int N, typename T>
inline void Vector<N, T>::setY(T y) {_v[1] = y;}

template <int N, typename T>
inline void Vector<N, T>::setZ(T z) {if(2 < N) _v[2] = z;}

template <int N, typename T>
inline void Vector<N, T>::setW(T w) {if(3 < N) _v[3] = w;}


//Operateurs surcharges
template <int N, typename T>
inline Vector<N, T> Vector<N, T>::operator+(const Vector<N, T>& vec) const
{
    return Vector<N, T>(*this) += vec;
}

template <int N, typename T>
inline Vector<N, T>& Vector<N, T>::operator+=(const Vector<N, T>& vec)
{
    for(int i=0; i<N; ++i)
        _v[i] += vec._v[i];
    return *this;
}

template <int N, typename T>
inline Vector<N, T> Vector<N, T>::operator-() const
{
    Vector<N, T> res(*this);
    for(int i=0; i<N; ++i)
        res[i] = -res[i];
    return res;
}

template <int N, typename T>
inline Vector<N, T> Vector<N, T>::operator-(const Vector<N, T>& vec) const
{
    return Vector<N, T>(*this) -= vec;
}

template <int N, typename T>
inline Vector<N, T>& Vector<N, T>::operator-=(const Vector<N, T>& vec)
{
    for(int i=0; i<N; ++i)
        _v[i] -= vec._v[i];
    return *this;
}

template <int N, typename T>
inline Vector<N, T> operator* (const T& coef, const Vector<N, T>& vec)
{
    return vec * coef;
}

template <int N, typename T>
inline Vector<N, T>  Vector<N, T>::operator* (const T& coef)   const
{
    return Vector<N, T>(*this) *= coef;
}

template <int N, typename T>
inline Vector<N, T>& Vector<N, T>::operator*=(const T& coef)
{
    for(int i=0; i<N; ++i)
        _v[i] *= coef;
    return *this;
}

template <int N, typename T>
inline Vector<N, T>  Vector<N, T>::operator/ (const T& coef) const
{
    return Vector<N, T>(*this) /= coef;
}

template <int N, typename T>
inline Vector<N, T>& Vector<N, T>::operator/=(const T& coef)
{
    if(coef != T(0))
    {
        for(int i=0; i<N; ++i)
            _v[i] /= coef;
    }
    return *this;
}

template <int N, typename T>
inline Vector<N, T>& Vector<N, T>::operator() (const T& x, const T& y, const T& z, const T& w)
{
    _v[0] = x;
    _v[1] = y;
    if(2 < N) _v[2] = z;
    if(3 < N) _v[3] = w;
    return *this;
}

template <int N, typename T>
inline T& Vector<N, T>::operator[](int axis)
{
    return _v[axis];
}

template <int N, typename T>
inline const T& Vector<N, T>::operator[](int axis) const
{
    return _v[axis];
}

template <int N, typename T>
inline bool Vector<N, T>::operator==(const Vector<N, T>& vec) const
{
    for(int i=0; i<N; ++i)
        if(std::abs(_v[i] - vec._v[i]) > FLT_EPSILON)
            return false;
    return true;
}

template <int N, typename T>
inline bool Vector<N, T>::operator!=(const Vector<N, T>& vec) const
{
    return !(*this == vec);
}

template <int N, typename T>
bool Vector<N, T>::operator <(const Vector<N, T>& vec) const
{
    for(int i=0; i < N; ++i)
    {
        if(_v[i] < vec[i])
            return true;
        if(vec[i] < _v[i])
            return false;
    }

    return false;
}

template <int N, typename T>
Vector<N, T>::operator bool() const
{
    for(int i=0; i < N; ++i)
        if(_v[i])
            return true;

    return false;
}

template <int N, typename T>
Vector<2, T> Vector<N, T>::polar(const T& angle, const T& radius)
{
    return Vector<2, T>(cos(angle) * radius, sin(angle) * radius);
}

template<int N, typename T>
inline Vector<N, T> multiply(const Vector<N,T>& vec1, const Vector<N,T>& vec2)
{
    Vector<N, T> res;
    for(int i=0; i<N; ++i)
        res[i] = vec1[i] * vec2[i];
    return res;
}

template<int N, typename T>
inline Vector<N,T> divide(const Vector<N,T>& vec1, const Vector<N,T>& vec2)
{
    Vector<N, T> res;
    for(int i=0; i<N; ++i)
        res[i] = vec1[i] / vec2[i];
    return res;
}

template<int N, typename T>
inline T dot(const Vector<N, T>& vec1, const Vector<N, T>& vec2)
{
    T res = T(0);
    for(int i=0; i<N; ++i)
        res += vec1[i] * vec2[i];
    return res;
}

template<typename T>
inline T cross(const Vector<2, T>& vec1, const Vector<2 ,T>& vec2)
{
    return vec1[0]*vec2[1] - vec1[1]*vec2[0];
}

template<typename T>
Vector<3, T> cross(const Vector<3, T>& vec1, const Vector<3, T>& vec2)
{
    Vector<3, T> res;
    res[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1];
    res[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
    res[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];
    return res;
}

template<int N, typename T >
inline Vector<N, T> proj(const Vector<N, T>& vec, const Vector<N, T>& unitVec)
{
    return dot(vec, unitVec) * unitVec;
}

template<typename T>
inline Vector<2,T> perpCW(const Vector<2,T>& vec)
{
    return Vector<2,T>(vec[1], -vec[0]);
}

template<typename T>
inline Vector<2,T> perpCCW(const Vector<2,T>& vec)
{
    return Vector<2,T>(-vec[1], vec[0]);
}

template<typename T>
Vector<2,T> rotate(const Vector<2,T>& vec, double radians)
{
    double s = sin(radians);
    double c = cos(radians);
    return Vector<2,T>(
                vec[0] * c - vec[1] * s,
            vec[0] * s + vec[1] * c
            );
}

template<int N, typename T >
std::ostream& operator<< (std::ostream& out, const Vector<N, T>& vec)
{
    out << '(';
    for(int i=0; i<N; ++i)
    {
        out << vec[i];
        if (i != N-1)
            out << ", ";
    }
    return out << ')';
}

template<int N, typename T >
std::istream& operator>> (std::istream& in, Vector<N, T>& vec)
{
    char c;
    for(int i=0; i<N; ++i)
        in >> c >> vec[i];
    return in;
}
}

#endif // CELLARWORKBENCH_VECTOR_H
