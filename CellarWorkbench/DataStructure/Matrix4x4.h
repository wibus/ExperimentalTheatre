#ifndef CELLARWORKBENCH_MATRIX4X4_H
#define CELLARWORKBENCH_MATRIX4X4_H

#include <cmath>
#include <cstring>
#include <algorithm>
#include <ostream>

#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix3x3.h"


namespace cellar
{
    template<typename T>
    class Matrix4x4
    {
    public:
        Matrix4x4();
        explicit Matrix4x4(T* values);
        explicit Matrix4x4(T diagonalValue);
        ~Matrix4x4();


        T* operator[](unsigned int j);
        const T* operator[](unsigned int j) const;

        T* asArray();
        const T* asArray() const;

        Matrix3x3<T> subMat3() const;

        Matrix4x4<T>& load(const Matrix4x4<T> mat);
        Matrix4x4<T>& loadIdentity();
        Matrix4x4<T>& translate(T x, T y, T z);
        Matrix4x4<T>& scale(T x, T y, T z);
        Matrix4x4<T>& rotate(T x, T y, T z, T radians);
        Matrix4x4<T>& transpose();
        Matrix4x4<T>  transposed() const;

        Matrix4x4<T>& rowOperation(int row1, T times1, int row2, T times2, int destRow);
        Matrix4x4<T>& multRow(int row, T val);
        Matrix4x4<T>& setRow(int row, const Vector4D<T> val);
        Matrix4x4<T>& setColomn(int col, const Vector4D<T> val);

        Matrix4x4<T>& ortho(T left,     T right,
                            T bottom,   T top,
                            T zNear,    T zFar);
        Matrix4x4<T>& frustrum(T left,      T right,
                               T bottom,    T top,
                               T zNear,     T zFar);
        Matrix4x4<T>& perspective(T fovyRadians, T aspect,
                                  T zNear,       T zFar);
        Matrix4x4<T>& lookAt(const Vector3D<T>& from,
                             const Vector3D<T>& to,
                             const Vector3D<T>& up);

        Vector4D<T>   operator *(const Vector4D<T>& vec) const;
        Matrix4x4<T>  operator *(const Matrix4x4<T>& mat) const;
        Matrix4x4<T>& operator*=(const Matrix4x4<T>& mat);
        Matrix4x4<T>  operator *(T val) const;
        Matrix4x4<T>& operator*=(T val);

        Matrix4x4<T>  operator +(const Matrix4x4<T>& mat) const;
        Matrix4x4<T>& operator+=(const Matrix4x4<T>& mat);

    private:
        T  _m[4][4];
    };

    typedef Matrix4x4<unsigned int> Mat4ui;
    typedef Matrix4x4<int>          Mat4i;
    typedef Matrix4x4<float>        Mat4f;
    typedef Matrix4x4<double>       Mat4d;

    template <typename T>
    std::ostream& operator<<(std::ostream& out, const Matrix4x4<T>& mat);



    // IMPLEMENTATION //
    template <typename T>
    Matrix4x4<T>::Matrix4x4()
    {
        loadIdentity();
    }

    template <typename T>
    Matrix4x4<T>::Matrix4x4(T* values)
    {
        _m[0][0] = values[0];  _m[0][1] = values[1];  _m[0][2] = values[2];  _m[0][3] = values[3];
        _m[1][0] = values[4];  _m[1][1] = values[5];  _m[1][2] = values[6];  _m[1][3] = values[7];
        _m[2][0] = values[8];  _m[2][1] = values[9];  _m[2][2] = values[10]; _m[2][3] = values[11];
        _m[3][0] = values[12]; _m[3][1] = values[13]; _m[3][2] = values[14]; _m[3][3] = values[15];
    }

    template <typename T>
    Matrix4x4<T>::Matrix4x4(T diagonalValue)
    {
        _m[0][0] = diagonalValue; _m[0][1] = 0; _m[0][2] = 0; _m[0][3] = 0;
        _m[1][0] = 0; _m[1][1] = diagonalValue; _m[1][2] = 0; _m[1][3] = 0;
        _m[2][0] = 0; _m[2][1] = 0; _m[2][2] = diagonalValue; _m[2][3] = 0;
        _m[3][0] = 0; _m[3][1] = 0; _m[3][2] = 0; _m[3][3] = diagonalValue;
    }

    template <typename T>
    Matrix4x4<T>::~Matrix4x4()
    {
    }


    template <typename T>
    inline T* Matrix4x4<T>::operator[](unsigned int j)
    {
        return const_cast<T*> (
            const_cast<const Matrix4x4<T>&>(*this)[j]
        );
    }

    template <typename T>
    inline const T* Matrix4x4<T>::operator[](unsigned int j) const
    {
        return _m[j];
    }

    template <typename T>
    T* Matrix4x4<T>::asArray()
    {
        return const_cast<T*> (
            const_cast<const Matrix4x4<T>&>(*this).asArray()
        );
    }

    template <typename T>
    const T* Matrix4x4<T>::asArray() const
    {
        return &_m[0][0];
    }

    template <typename T>
    Matrix3x3<T> Matrix4x4<T>::subMat3() const
    {
        Matrix3x3<T> mat3;
        mat3[0][0] = _m[0][0]; mat3[0][1] = _m[0][1]; mat3[0][2] = _m[0][2];
        mat3[1][0] = _m[1][0]; mat3[1][1] = _m[1][1]; mat3[1][2] = _m[1][2];
        mat3[2][0] = _m[2][0]; mat3[2][1] = _m[2][1]; mat3[2][2] = _m[2][2];
        return mat3;
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::load(const Matrix4x4<T> mat)
    {
        _m[0][0] = mat._m[0][0]; _m[0][1] = mat._m[0][1]; _m[0][2] = mat._m[0][2]; _m[0][3] = mat._m[0][3];
        _m[1][0] = mat._m[1][0]; _m[1][1] = mat._m[1][1]; _m[1][2] = mat._m[1][2]; _m[1][3] = mat._m[1][3];
        _m[2][0] = mat._m[2][0]; _m[2][1] = mat._m[2][1]; _m[2][2] = mat._m[2][2]; _m[2][3] = mat._m[2][3];
        _m[3][0] = mat._m[3][0]; _m[3][1] = mat._m[3][1]; _m[3][2] = mat._m[3][2]; _m[3][3] = mat._m[3][3];
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::loadIdentity()
    {
        _m[0][0] = 1; _m[0][1] = 0; _m[0][2] = 0; _m[0][3] = 0;
        _m[1][0] = 0; _m[1][1] = 1; _m[1][2] = 0; _m[1][3] = 0;
        _m[2][0] = 0; _m[2][1] = 0; _m[2][2] = 1; _m[2][3] = 0;
        _m[3][0] = 0; _m[3][1] = 0; _m[3][2] = 0; _m[3][3] = 1;
        return *this;
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::translate(T x, T y, T z)
    {
        Matrix4x4<T> transMat;
        transMat._m[0][3] = x;
        transMat._m[1][3] = y;
        transMat._m[2][3] = z;

        return *this *= transMat;
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::scale(T x, T y, T z)
    {
        Matrix4x4<T> scaleMat;
        scaleMat._m[0][0] = x;
        scaleMat._m[1][1] = y;
        scaleMat._m[2][2] = z;

        return *this *= scaleMat;
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::rotate(T x, T y, T z, T radians)
    {
        Matrix4x4<T> rotMat;

        T c = std::cos(radians);
        T s = std::sin(radians);
        T t = 1 - c;

        rotMat._m[0][0] = t*x*x + c;   rotMat._m[0][1] = t*x*y - s*z; rotMat._m[0][2] = t*x*z + s*y;
        rotMat._m[1][0] = t*x*y + s*z; rotMat._m[1][1] = t*y*y + c;   rotMat._m[1][2] = t*y*z - s*x;
        rotMat._m[2][0] = t*x*z - s*y; rotMat._m[2][1] = t*y*z + s*x; rotMat._m[2][2] = t*z*z + c;

        return *this *= rotMat;
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::transpose()
    {
        //    0 1 2 3
        //    -------
        // 0| x 0 0 0
        // 1| 0 x 0 0
        // 2| 0 0 x 0
        // 3| 0 0 0 x

        std::swap(_m[1][0], _m[0][1]);
        std::swap(_m[2][0], _m[0][2]);
        std::swap(_m[2][1], _m[1][2]);
        std::swap(_m[3][0], _m[0][3]);
        std::swap(_m[3][1], _m[1][3]);
        std::swap(_m[3][2], _m[2][3]);

        return *this;
    }

    template <typename T>
    Matrix4x4<T> Matrix4x4<T>::transposed() const
    {
        return Matrix4x4<T>(*this).transpose();
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::rowOperation(int row1, T times1, int row2, T times2, int destRow)
    {
        return setRow(destRow,
               Vector4D<T>(_m[row1]) * times1 +
               Vector4D<T>(_m[row2]) * times2);
    }

    template <typename T>
    inline Matrix4x4<T>& Matrix4x4<T>::multRow(int row, T val)
    {
        _m[row][0] *= val; _m[row][1] *= val; _m[row][2] *= val; _m[row][3] *= val;
        return *this;
    }

    template <typename T>
    inline Matrix4x4<T>& Matrix4x4<T>::setRow(int row, const Vector4D<T> val)
    {
        _m[row][0] = val[0]; _m[row][1] = val[1]; _m[row][2] = val[2]; _m[row][3] = val[3];
        return *this;
    }

    template <typename T>
    inline Matrix4x4<T>& Matrix4x4<T>::setColomn(int col, const Vector4D<T> val)
    {
        _m[0][col] = val[0]; _m[1][col] = val[1]; _m[2][col] = val[2]; _m[3][col] = val[3];
        return *this;
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::ortho(T left,     T right,
                                      T bottom,   T top,
                                      T zNear,    T zFar)
    {
        Matrix4x4<T> ortho;
        ortho._m[0][0] = 2 / (right - left);
        ortho._m[1][1] = 2 / (top   - bottom);
        ortho._m[2][2] = 2 / (zNear - zFar);

        ortho._m[0][3] = -(right + left)   / (right - left);
        ortho._m[1][3] = -(top   + bottom) / (top   - bottom);
        ortho._m[2][3] = -(zFar  + zNear)  / (zFar  - zNear);

        return *this *= ortho;
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::frustrum(T left,      T right,
                                         T bottom,    T top,
                                         T zNear,     T zFar)
    {
        T zNear2 = 2*zNear;

        Matrix4x4<T> frust;
        frust._m[0][0] = zNear2/(right-left);
        frust._m[1][1] = zNear2/(top-bottom);
        frust._m[2][2] = (zFar+zNear)/(zNear-zFar);
        frust._m[3][3] = 0;

        frust._m[0][2] = (right+left)/(right-left);
        frust._m[1][2] = (top+bottom)/(top-bottom);
        frust._m[3][2] = -1;
        frust._m[2][3] = -zNear2*zFar/(zFar-zNear);

        return *this *= frust;
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::perspective(T fovyRadians, T aspect,
                                            T zNear,       T zFar)
    {
        T halfHeight = zNear * atan(fovyRadians/2);
        T halfWidth = aspect * halfHeight;
        frustrum(-halfWidth,  halfWidth,
                 -halfHeight, halfHeight,
                 zNear,       zFar);
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::lookAt(const Vector3D<T>& from,
                                       const Vector3D<T>& to,
                                       const Vector3D<T>& up)
    {
        Vector3D<T> z = (from - to).normalize();
        Vector3D<T> x = (up ^ z).normalize();
        Vector3D<T> y = (z ^ x).normalize();

        Matrix4x4<T> m;
        m._m[0][0] = x[0]; m._m[0][1] = x[1]; m._m[0][2] = x[2]; m._m[0][3] = -(x * from);
        m._m[1][0] = y[0]; m._m[1][1] = y[1]; m._m[1][2] = y[2]; m._m[1][3] = -(y * from);
        m._m[2][0] = z[0]; m._m[2][1] = z[1]; m._m[2][2] = z[2]; m._m[2][3] = -(z * from);
        m._m[3][0] = 0;    m._m[3][1] = 0;    m._m[3][2] = 0;    m._m[3][3] = 1;

        return *this *= m;
    }

    template <typename T>
    Vector4D<T>  Matrix4x4<T>::operator *(const Vector4D<T>& vec) const
    {
        return Vector4D<T>(
            Vector4D<T>(_m[0]) * vec,
            Vector4D<T>(_m[1]) * vec,
            Vector4D<T>(_m[2]) * vec,
            Vector4D<T>(_m[3]) * vec
        );
    }

    template <typename T>
    inline Matrix4x4<T> Matrix4x4<T>::operator *(const Matrix4x4<T>& mat) const
    {
        return Matrix4x4<T>(*this) *= mat;
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::operator*=(const Matrix4x4<T>& mat)
    {
        Vector4D<T> rows[4];
        rows[0] = Vector4D<T>(_m[0]);
        rows[1] = Vector4D<T>(_m[1]);
        rows[2] = Vector4D<T>(_m[2]);
        rows[3] = Vector4D<T>(_m[3]);

        Vector4D<T> colomns[4];
        colomns[0] = Vector4D<T>(mat._m[0][0], mat._m[1][0], mat._m[2][0], mat._m[3][0]);
        colomns[1] = Vector4D<T>(mat._m[0][1], mat._m[1][1], mat._m[2][1], mat._m[3][1]);
        colomns[2] = Vector4D<T>(mat._m[0][2], mat._m[1][2], mat._m[2][2], mat._m[3][2]);
        colomns[3] = Vector4D<T>(mat._m[0][3], mat._m[1][3], mat._m[2][3], mat._m[3][3]);

        _m[0][0] = rows[0] * colomns[0];
        _m[0][1] = rows[0] * colomns[1];
        _m[0][2] = rows[0] * colomns[2];
        _m[0][3] = rows[0] * colomns[3];

        _m[1][0] = rows[1] * colomns[0];
        _m[1][1] = rows[1] * colomns[1];
        _m[1][2] = rows[1] * colomns[2];
        _m[1][3] = rows[1] * colomns[3];

        _m[2][0] = rows[2] * colomns[0];
        _m[2][1] = rows[2] * colomns[1];
        _m[2][2] = rows[2] * colomns[2];
        _m[2][3] = rows[2] * colomns[3];

        _m[3][0] = rows[3] * colomns[0];
        _m[3][1] = rows[3] * colomns[1];
        _m[3][2] = rows[3] * colomns[2];
        _m[3][3] = rows[3] * colomns[3];

        return *this;
    }

    template <typename T>
    inline Matrix4x4<T> Matrix4x4<T>::operator *(T val) const
    {
        return Matrix4x4<T>(*this) *= val;
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::operator*=(T val)
    {
        _m[0][0] *= val; _m[0][1] *= val; _m[0][2] *= val; _m[0][3] *= val;
        _m[1][0] *= val; _m[1][1] *= val; _m[1][2] *= val; _m[1][3] *= val;
        _m[2][0] *= val; _m[2][1] *= val; _m[2][2] *= val; _m[2][3] *= val;
        _m[3][0] *= val; _m[3][1] *= val; _m[3][2] *= val; _m[3][3] *= val;

        return *this;
    }


    template <typename T>
    inline Matrix4x4<T> Matrix4x4<T>::operator +(const Matrix4x4<T>& mat) const
    {
        return Matrix4x4<T>(*this) += mat;
    }

    template <typename T>
    Matrix4x4<T>& Matrix4x4<T>::operator+=(const Matrix4x4<T>& mat)
    {
        _m[0][0] += _m[0][0]; _m[0][1] += _m[0][1]; _m[0][2] += _m[0][2]; _m[0][3] += _m[0][3];
        _m[1][0] += _m[1][0]; _m[1][1] += _m[1][1]; _m[1][2] += _m[1][2]; _m[1][3] += _m[1][3];
        _m[2][0] += _m[2][0]; _m[2][1] += _m[2][1]; _m[2][2] += _m[2][2]; _m[2][3] += _m[2][3];
        _m[3][0] += _m[3][0]; _m[3][1] += _m[3][1]; _m[3][2] += _m[3][2]; _m[3][3] += _m[3][3];
    }    

    template <typename T>
    std::ostream& operator<<(std::ostream& out, const Matrix4x4<T>& mat)
    {
        out << mat[0][0] << '\t' << mat[0][1] << '\t' << mat[0][2] << '\t' << mat[0][3] << std::endl
            << mat[1][0] << '\t' << mat[1][1] << '\t' << mat[1][2] << '\t' << mat[1][3] << std::endl
            << mat[2][0] << '\t' << mat[2][1] << '\t' << mat[2][2] << '\t' << mat[2][3] << std::endl
            << mat[3][0] << '\t' << mat[3][1] << '\t' << mat[3][2] << '\t' << mat[3][3] << std::endl;
        return out;
    }
}

#endif // CELLARWORKBENCH_MATRIX4X4_H
