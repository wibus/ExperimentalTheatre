#ifndef CELLARWORKBENCH_MATRIX3X3_H
#define CELLARWORKBENCH_MATRIX3X3_H

#include "Vector.h"


namespace cellar
{
    template<typename T>
    class Matrix3x3
    {
    public:
        Matrix3x3();
        explicit Matrix3x3(const T* values);
        explicit Matrix3x3(T diagonalValue);
        ~Matrix3x3();

        void loadIdentity();
        void loadDiagonal(T d);
        void loadDiagonal(const T* d);
        void loadValues(const T* values);

        T* asArray();
        const T* asArray() const;

        T* operator[](unsigned int j);
        const T* operator[](unsigned int j) const;

        Vector<3, T> operator* (const Vector<3, T>& vec) const;
        Matrix3x3<T>& operator*=(const Matrix3x3<T>& mat);
        Matrix3x3<T>  operator* (const Matrix3x3<T>& mat) const;
        Matrix3x3<T>& operator+=(const Matrix3x3<T>& mat);
        Matrix3x3<T>  operator+ (const Matrix3x3<T>& mat) const;
        Matrix3x3<T>& operator-=(const Matrix3x3<T>& mat);
        Matrix3x3<T>  operator- (const Matrix3x3<T>& mat) const;
        Matrix3x3<T>& operator- ();

        Matrix3x3<T>& translate(const Vector<3, T>& displacement);
        Matrix3x3<T>  translated(const Vector<3, T>& displacement) const;
        Matrix3x3<T>& scale(const T& factor);
        Matrix3x3<T>  scaled(const T& factor) const;
        Matrix3x3<T>& rotate(const T& angle);
        Matrix3x3<T>  rotated(const T& angle) const;

        T determinant() const;
        Matrix3x3<T>& transpose();
        Matrix3x3<T>  transposed() const;
        Matrix3x3<T>& inverse();
        Matrix3x3<T>  inversed() const;

    private:
        T _m[3][3];
    };

    typedef Matrix3x3<unsigned int> Mat3ui;
    typedef Matrix3x3<int>          Mat3i;
    typedef Matrix3x3<float>        Mat3f;
    typedef Matrix3x3<double>       Mat3d;


    // IMPLEMENTATION //
    template<typename T>
    Matrix3x3<T>::Matrix3x3()
    {
        loadIdentity();
    }

    template<typename T>
    Matrix3x3<T>::Matrix3x3(const T* values)
    {
        loadValues(values);
    }

    template<typename T>
    Matrix3x3<T>::Matrix3x3(T diagonalValue)
    {
        loadDiagonal(diagonalValue);
    }

    template<typename T>
    Matrix3x3<T>::~Matrix3x3()
    {
    }

    template<typename T>
    inline void Matrix3x3<T>::loadIdentity()
    {
        loadDiagonal(1);
    }

    template<typename T>
    void Matrix3x3<T>::loadDiagonal(T d)
    {
        _m[0][0] = d; _m[0][1] = 0; _m[0][2] = 0;
        _m[1][0] = 0; _m[1][1] = d; _m[1][2] = 0;
        _m[2][0] = 0; _m[2][1] = 0; _m[2][2] = d;
    }

    template<typename T>
    void Matrix3x3<T>::loadDiagonal(const T* d)
    {
        _m[0][0] = d[0]; _m[0][1] = 0;    _m[0][2] = 0;
        _m[1][0] = 0;    _m[1][1] = d[1]; _m[1][2] = 0;
        _m[2][0] = 0;    _m[2][1] = 0;    _m[2][2] = d[2];
    }

    template<typename T>
    void Matrix3x3<T>::loadValues(const T* values)
    {
        _m[0][0] = values[0]; _m[0][1] = values[1]; _m[0][2] = values[2];
        _m[1][0] = values[3]; _m[1][1] = values[4]; _m[1][2] = values[5];
        _m[2][0] = values[6]; _m[2][1] = values[7]; _m[2][2] = values[8];
    }

    template<typename T>
    inline T* Matrix3x3<T>::asArray()
    {
        return &_m[0][0];
    }

    template<typename T>
    inline const T* Matrix3x3<T>::asArray() const
    {
        return &_m[0][0];
    }

    template<typename T>
    inline T* Matrix3x3<T>::operator[](unsigned int j)
    {
        return _m[j];
    }

    template<typename T>
    inline const T* Matrix3x3<T>::operator[](unsigned int j) const
    {
        return _m[j];
    }

    template<typename T>
    Vector<3, T> Matrix3x3<T>::operator* (const Vector<3, T>& vec) const
    {
        return Vector<3, T>(
            dot(Vector<3, T>(_m[0]), vec),
            dot(Vector<3, T>(_m[1]), vec),
            dot(Vector<3, T>(_m[2]), vec)
        );
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::operator*=(const Matrix3x3<T>& mat)
    {
        Matrix3x3<T> old(*this);

        for(int j=0; j<3; ++j)
        {
            for(int i=0; i<3; ++i)
            {
                _m[j][i] = T(0);

                for(int k=0; k<3; ++k)
                {
                    _m[j][i] += old._m[j][k] * mat._m[k][i];
                }
            }
        }

        return *this;
    }

    template<typename T>
    inline Matrix3x3<T>  Matrix3x3<T>::operator* (const Matrix3x3<T>& mat) const
    {
        return (Matrix3x3<T>(*this) *= mat);
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::operator+=(const Matrix3x3<T>& mat)
    {
        _m[0][0] += mat._m[0][0];
        _m[0][1] += mat._m[0][1];
        _m[0][2] += mat._m[0][2];

        _m[1][0] += mat._m[1][0];
        _m[1][1] += mat._m[1][1];
        _m[1][2] += mat._m[1][2];

        _m[2][0] += mat._m[2][0];
        _m[2][1] += mat._m[2][1];
        _m[2][2] += mat._m[2][2];
    }

    template<typename T>
    inline Matrix3x3<T>  Matrix3x3<T>::operator+ (const Matrix3x3<T>& mat) const
    {
        return (Matrix3x3<T>(*this) += mat);
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::operator-=(const Matrix3x3<T>& mat)
    {
        _m[0][0] -= mat._m[0][0];
        _m[0][1] -= mat._m[0][1];
        _m[0][2] -= mat._m[0][2];

        _m[1][0] -= mat._m[1][0];
        _m[1][1] -= mat._m[1][1];
        _m[1][2] -= mat._m[1][2];

        _m[2][0] -= mat._m[2][0];
        _m[2][1] -= mat._m[2][1];
        _m[2][2] -= mat._m[2][2];
    }

    template<typename T>
    inline Matrix3x3<T>  Matrix3x3<T>::operator- (const Matrix3x3<T>& mat) const
    {
        return (Matrix3x3<T>(*this) -= mat);
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::operator- ()
    {
        _m[0][0] = -_m[0][0];
        _m[0][1] = -_m[0][1];
        _m[0][2] = -_m[0][2];

        _m[1][0] = -_m[1][0];
        _m[1][1] = -_m[1][1];
        _m[1][2] = -_m[1][2];

        _m[2][0] = -_m[2][0];
        _m[2][1] = -_m[2][1];
        _m[2][2] = -_m[2][2];
    }

    template<typename T>
    inline Matrix3x3<T>& Matrix3x3<T>::translate(const Vector<3, T>& displacement)
    {
        _m[0][2] += displacement.x();
        _m[1][2] += displacement.y();
        _m[2][2] += displacement.z();

        return *this;
    }

    template<typename T>
    inline Matrix3x3<T>  Matrix3x3<T>::translated(const Vector<3, T>& displacement) const
    {
        return Matrix3x3<T>(*this).translate(displacement);
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::scale(const T& factor)
    {
        _m[0][0] *= factor; _m[0][1] *= factor; _m[0][2] *= factor;
        _m[1][0] *= factor; _m[1][1] *= factor; _m[1][2] *= factor;

        return *this;
    }

    template<typename T>
    inline Matrix3x3<T>  Matrix3x3<T>::scaled(const T& factor) const
    {
        return Matrix3x3<T>(*this).scale(factor);
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::rotate(const T& angle)
    {
        Matrix3x3<T> rotation;
        rotation._m[0][0] = cos(angle); rotation._m[0][1] = -sin(angle);
        rotation._m[1][0] = sin(angle); rotation._m[1][1] = cos(angle);

        return (*this *= rotation);
    }

    template<typename T>
    inline Matrix3x3<T>  Matrix3x3<T>::rotated(const T& angle) const
    {
        return Matrix3x3<T>(*this).rotate(angle);
    }

    template<typename T>
    T Matrix3x3<T>::determinant() const
    {
        return _m[0][0] * (_m[1][1]*_m[2][2] - _m[1][2]*_m[2][1]) -
               _m[1][0] * (_m[0][1]*_m[2][2] - _m[0][2]*_m[2][1]) +
               _m[2][0] * (_m[0][1]*_m[1][2] - _m[0][2]*_m[1][1]);
    }

    template<typename T>
    inline Matrix3x3<T>& Matrix3x3<T>::transpose()
    {
        std::swap(_m[0][1], _m[1][0]);
        std::swap(_m[0][2], _m[2][0]);
        std::swap(_m[1][2], _m[2][1]);

        return *this;
    }

    template<typename T>
    inline Matrix3x3<T>  Matrix3x3<T>::transposed() const
    {
        return Matrix3x3<T>(*this).transpose();
    }

    template<typename T>
    Matrix3x3<T>& Matrix3x3<T>::inverse()
    {
        // TODO: wibus 2012-09-23: will certainly need a Matrix2x2 to help
        return *this;
    }

    template<typename T>
    inline Matrix3x3<T>  Matrix3x3<T>::inversed() const
    {
        return Matrix3x3<T>(*this).inverse();
    }
}

#endif // CELLARWORKBENCH_MATRIX3X3_H
