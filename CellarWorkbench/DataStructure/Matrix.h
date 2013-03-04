#ifndef CELLARWORKBENCH_MATRIX_H
#define CELLARWORKBENCH_MATRIX_H

#include "Vector.h"


namespace cellar
{
    template <int N, typename T>
    class Matrix
    {
    public:
        Matrix();
        explicit Matrix(const T* values);
        explicit Matrix(const T& diagonalValue);
        template <int N2, typename T2> Matrix(const Matrix<N2, T2>& rhs);
        ~Matrix();


        T*       asArray();
        const T* asArray() const;
        Vector<N, T>&       operator[](unsigned int i);
        const Vector<N, T>& operator[](unsigned int i) const;


        Vector<N, T>  operator* (const Vector<N, T>& rhs) const;
        Matrix<N, T>& operator*=(const Matrix<N, T>& rhs);
        Matrix<N, T>  operator* (const Matrix<N, T>& rhs) const;
        Matrix<N, T>& operator+=(const Matrix<N, T>& rhs);
        Matrix<N, T>  operator+ (const Matrix<N, T>& rhs) const;
        Matrix<N, T>& operator-=(const Matrix<N, T>& rhs);
        Matrix<N, T>  operator- (const Matrix<N, T>& rhs) const;
        Matrix<N, T>& operator- ();
        Matrix<N, T>& operator*=(const T& coef);
        Matrix<N, T>  operator* (const T& coef) const;
        Matrix<N, T>& operator/=(const T& coef);
        Matrix<N, T>  operator/ (const T& coef) const;


        Matrix<N, T>& loadIdentity();
        Matrix<N, T>& loadValues(const T* values);
        Matrix<N, T>& loadDiagonal(T diagonalValue);
        Matrix<N, T>& loadDiagonal(const T* diagonalValues);
        Matrix<N, T>& setRow(int i, const Vector<N, T>& row);
        Matrix<N, T>& setRow(int i, const T& i1, const T& i2, const T& i3=0, const T& i4=0);
        Matrix<N, T>& setColumn(int j, const Vector<N, T>& column);
        Matrix<N, T>& setColumn(int j, const T& j1, const T& j2, const T& j3=0, const T& j4=0);


    private:
        Vector<N, T> _m[N]; // Line vectors
    };

    typedef Matrix<2, int> Mat2i;
    typedef Matrix<2, float> Mat2f;
    typedef Matrix<2, double> Mat2d;

    typedef Matrix<3, int> Mat3i;
    typedef Matrix<3, float> Mat3f;
    typedef Matrix<3, double> Mat3d;

    typedef Matrix<4, int> Mat4i;
    typedef Matrix<4, float> Mat4f;
    typedef Matrix<4, double> Mat4d;

    template <int N, typename T>
    T det(const Matrix<N,T>& mat);
    template <int N, typename T>
    T matminor(const Matrix<N,T>& mat, int i, int j);
    template <int N, typename T>
    T cofactor(const Matrix<N,T>& mat, int i, int j);
    template <int N, typename T>
    Matrix<N,T> transpose(const Matrix<N,T>& mat);
    template <int N, typename T>
    Matrix<N-1,T> submat(const Matrix<N,T>& mat, int ommitRow, int ommitColumn);
    template <int N, typename T>
    Matrix<N,T> adjugate(const Matrix<N,T>& mat);
    template <int N, typename T>
    Matrix<N,T> inverse(const Matrix<N,T>& mat);

    template <typename T>
    Matrix<3,T> translate(const T& tx, const T& ty);
    template <typename T>
    Matrix<4,T> translate(const T& tx, const T& ty, const T& tz);
    template <typename T>
    Matrix<2,T> scale(const T& sx, const T& sy);
    template <typename T>
    Matrix<3,T> scale(const T& sx, const T& sy, const T& sz);
    template <typename T>
    Matrix<4,T> scale(const T& sx, const T& sy, const T& sz, const T& sw);
    template <typename T>
    Matrix<3,T> rotate(const T& radians);
    template <typename T>
    Matrix<4,T> rotate(const T& rx, const T& ry, const T& rz, const T& radians);

    template <typename T>
    Matrix<4,T> ortho(const T& left,   const T& right,
                      const T& bottom, const T& top,
                      const T& zNear,  const T& zFar);
    template <typename T>
    Matrix<4,T> frustrum(const T& left,   const T& right,
                         const T& bottom, const T& top,
                         const T& zNear,  const T& zFar);
    template <typename T>
    Matrix<4,T> perspective(const T& fovyRadians, const T& aspect,
                            const T& zNear,       const T& zFar);
    template <typename T>
    Matrix<4,T> lookAt(const Vector<3, T>& from,
                       const Vector<3, T>& to,
                       const Vector<3, T>& up);

    template<int N, typename T >
    std::ostream& operator<< (std::ostream& out, const Matrix<N,T>& mat);
    template<int N, typename T >
    std::istream& operator>> (std::istream& in, Matrix<N,T>& mat);


    // IMPLEMENTATION //
    template <int N, typename T>
    Matrix<N,T>::Matrix()
    {
        loadIdentity();
    }

    template <int N, typename T>
    Matrix<N,T>::Matrix(const T* values)
    {
        loadValues(values);
    }

    template <int N, typename T>
    Matrix<N,T>::Matrix(const T& diagonalValue)
    {
        loadDiagonal(diagonalValue);
    }

    template <int N, typename T>
    template <int N2, typename T2>
    Matrix<N,T>::Matrix(const Matrix<N2, T2>& rhs)
    {
        const int MIN_DIM = N < N2 ? N : N2;
        for(int i=0; i<MIN_DIM; ++i)
        {
            for(int j=0; j<MIN_DIM; ++j)
            {
                _m[i][j] = rhs[i][j];
            }
        }
        for(int i=MIN_DIM; i<N; ++i)
        {
            for(int j=MIN_DIM; j<N; ++j)
            {
                if(i == j) _m[i][j] = T(1);
                else       _m[i][j] = T(0);
            }
        }
    }

    template <int N, typename T>
    Matrix<N,T>::~Matrix()
    {

    }


    template <int N, typename T>
    T* Matrix<N,T>::asArray()
    {
        return &_m;
    }

    template <int N, typename T>
    const T* Matrix<N,T>::asArray() const
    {
        return (T*)this;
    }

    template <int N, typename T>
    Vector<N, T>& Matrix<N,T>::operator[](unsigned int i)
    {
        return _m[i];
    }

    template <int N, typename T>
    const Vector<N, T>& Matrix<N,T>::operator[](unsigned int i) const
    {
        return _m[i];
    }


    template <int N, typename T>
    Vector<N, T> Matrix<N,T>::operator* (const Vector<N, T>& rhs) const
    {
        Vector<N, T> res;
        for(int i=0; i<N; ++i)
            res[i] = dot(_m[i], rhs);
        return res;
    }


    template <int N, typename T>
    Matrix<N, T>& Matrix<N,T>::operator*=(const Matrix<N, T>& rhs)
    {
        T res[N][N];
        for(int i=0; i<N; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                res[i][j] = T(0);
                for(int k=0; k<N; ++k)
                {
                    res[i][j] += _m[i][k] * rhs[k][j];
                }
            }
        }
        loadValues(&res[0][0]);
        return *this;
    }

    template <int N, typename T>
    Matrix<N, T> Matrix<N,T>::operator* (const Matrix<N, T>& rhs) const
    {
        T res[N][N];
        for(int i=0; i<N; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                res[i][j] = T(0);
                for(int k=0; k<N; ++k)
                {
                    res[i][j] += _m[i][k] * rhs[k][j];
                }
            }
        }
        return Matrix<N, T>(res);
    }

    template <int N, typename T>
    Matrix<N, T>& Matrix<N,T>::operator+=(const Matrix<N, T>& rhs)
    {
        for(int i=0; i<N; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                _m[i][j] += rhs[i][j];
            }
        }
        return this;
    }

    template <int N, typename T>
    Matrix<N, T> Matrix<N,T>::operator+ (const Matrix<N, T>& rhs) const
    {
        return Matrix(*this) += rhs;
    }

    template <int N, typename T>
    Matrix<N, T>& Matrix<N,T>::operator-=(const Matrix<N, T>& rhs)
    {
        for(int i=0; i<N; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                _m[i][j] -= rhs[i][j];
            }
        }
    }

    template <int N, typename T>
    Matrix<N, T> Matrix<N,T>::operator- (const Matrix<N, T>& rhs) const
    {
        return Matrix(*this) -= rhs;
    }

    template <int N, typename T>
    Matrix<N, T>& Matrix<N,T>::operator- ()
    {
        for(int i=0; i<N; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                _m[i][j] = -_m[i][j];
            }
        }
        return *this;
    }

    template <int N, typename T>
    Matrix<N, T>& Matrix<N,T>::operator*=(const T& coef)
    {
        for(int i=0; i<N; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                _m[i][j] *= coef;
            }
        }
        return *this;
    }

    template <int N, typename T>
    inline Matrix<N, T> Matrix<N,T>::operator* (const T& coef) const
    {
        return Matrix<N, T>(&_m[0][0]) *= coef;
    }

    template <int N, typename T>
    inline Matrix<N, T> operator* (const T& coef, const Matrix<N, T>& mat)
    {
        return mat * coef;
    }

    template <int N, typename T>
    Matrix<N, T>& Matrix<N,T>::operator/=(const T& coef)
    {
        for(int i=0; i<N; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                _m[i][j] /= coef;
            }
        }
        return *this;
    }

    template <int N, typename T>
    inline Matrix<N, T> Matrix<N,T>::operator/ (const T& coef) const
    {
        return Matrix<N, T>(&_m[0][0]) /= coef;
    }

    template <int N, typename T>
    inline Matrix<N, T> operator/ (const T& coef, const Matrix<N, T>& mat)
    {
        return mat / coef;
    }

    template <int N, typename T>
    Matrix<N,T>& Matrix<N,T>::loadIdentity()
    {
        for(int i=0; i<N; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                if(i == j) _m[i][j] = T(1);
                else       _m[i][j] = T(0);
            }
        }
        return *this;
    }

    template <int N, typename T>
    Matrix<N,T>& Matrix<N,T>::loadValues(const T* values)
    {
        for(int i=0; i<N; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                _m[i][j] = values[i*N+j];
            }
        }
        return *this;
    }

    template <int N, typename T>
    Matrix<N,T>& Matrix<N,T>::loadDiagonal(T diagonalValue)
    {
        for(int i=0; i<N; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                if(i == j) _m[i][j] = diagonalValue;
                else       _m[i][j] = T(0);
            }
        }
        return *this;
    }

    template <int N, typename T>
    Matrix<N,T>& Matrix<N,T>::loadDiagonal(const T* diagonalValues)
    {
        for(int i=0; i<N; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                if(i == j) _m[i][j] = diagonalValues[i];
                else       _m[i][j] = T(0);
            }
        }
        return *this;
    }

    template <int N, typename T>
    inline Matrix<N,T>& Matrix<N,T>::setRow(int i, const Vector<N, T>& row)
    {
        _m[i] = row;
        return *this;
    }

    template <int N, typename T>
    inline Matrix<N,T>& Matrix<N,T>::setRow(int i, const T& i1, const T& i2, const T& i3, const T& i4)
    {
        _m[i](i1, i2, i3, i4);
        return *this;
    }

    template <int N, typename T>
    inline Matrix<N,T>& Matrix<N,T>::setColumn(int j, const Vector<N, T>& column)
    {
        for(int i=0; i<N; ++i)
            _m[i][j] = column[i];
        return *this;
    }

    template <int N, typename T>
    inline Matrix<N,T>& Matrix<N,T>::setColumn(int j, const T& j1, const T& j2, const T& j3, const T& j4)
    {
        _m[0][j] = j1;
        _m[1][j] = j2;
        if(2 < N) _m[2][j] = j3;
        if(3 < N) _m[3][j] = j4;
        return *this;
    }

    template <typename T>
    T det(const Matrix<1,T>& mat)
    {
        return mat[0][0];
    }

    template <typename T>
    T det(const Matrix<2,T>& mat)
    {
        return mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0];
    }

    template <int N, typename T>
    T det(const Matrix<N,T>& mat)
    {
        return mat[N-1][N-1] * det(submat(mat, N-1, N-1));
    }

    template <int N, typename T>
    T matminor(const Matrix<N,T>& mat, int i, int j)
    {
        return det(submat(mat, i, j));
    }

    template <int N, typename T>
    T cofactor(const Matrix<N,T>& mat, int i, int j)
    {
        if(i+j % 2)
            return -matminor(mat, i, j);
        return matminor(mat, i, j);
    }

    template <int N, typename T>
    Matrix<N,T> transpose(const Matrix<N,T>& mat)
    {
        T trans[N][N];
        for(int i=0; i<N; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                trans[i][j] = mat[j][i];
            }
        }
        return Matrix<N,T>(&trans[0][0]);
    }

    template <int N, typename T>
    Matrix<N-1,T> submat(const Matrix<N,T>& mat, int ommitRow, int ommitColumn)
    {
        T sub[N-1][N-1];
        for(int i=0; i<N; ++i)
        {
            int si = i;
            if(i == ommitRow) continue;
            if(ommitRow < i) --si;
            for(int j=0; j<N; ++j)
            {
                int sj = j;
                if(j == ommitColumn) continue;
                if(ommitColumn < j) --sj;
                sub[si][sj] = mat[i][j];
            }
        }
        return Matrix<N-1,T>(&sub[0][0]);
    }

    template <int N, typename T>
    Matrix<N,T> adjugate(const Matrix<N,T>& mat)
    {
        T cofs[N][N];
        for(int i=0; i<N; ++i)
        {
            for(int j=0; j<N; ++j)
            {
                cofs[i][j] = cofactor(mat, i, j);
            }
        }
        return transpose(Matrix<N,T>(&cofs[0][0]));
    }

    template <int N, typename T>
    Matrix<N,T> inverse(const Matrix<N,T>& mat)
    {
        return adjugate(mat) / det(mat);
    }

    template <typename T>
    inline Matrix<3,T> translate(const T& tx, const T& ty)
    {
        Matrix<3,T> res;
        res[0][2] = tx;
        res[1][2] = ty;
        return res;
    }

    template <typename T>
    inline Matrix<4,T> translate(const T& tx, const T& ty, const T& tz)
    {
        Matrix<4,T> res;
        res[0][3] = tx;
        res[1][3] = ty;
        res[2][3] = tz;
        return res;
    }

    template <typename T>
    inline Matrix<2,T> scale(const T& sx, const T& sy=T(1))
    {
        T values[] = {
            sx, T(0),
            sy, T(0)
        };
        return Matrix<2,T>(values);
    }

    template <typename T>
    Matrix<3,T> scale(const T& sx, const T& sy, const T& sz=T(1))
    {
        T values[] = {
            sx,   T(0), T(0),
            T(0), sy,   T(0),
            T(0), T(0), sz
        };
        return Matrix<3,T>(values);
    }

    template <typename T>
    Matrix<4,T> scale(const T& sx, const T& sy, const T& sz, const T& sw=T(1))
    {
        T values[] = {
            sx,   T(0), T(0), T(0),
            T(0), sy,   T(0), T(0),
            T(0), T(0), sz,   T(0),
            T(0), T(0), T(0), sw
        };
        return Matrix<4,T>(values);
    }

    template <typename T>
    Matrix<3,T> rotate(const T& radians)
    {
        T s = sin(radians);
        T c = cos(radians);
        T values[] = {
            c, -s,
            s, c
        };
        return Matrix<2,T>(values);
    }

    template <typename T>
    Matrix<4,T> rotate(const T& rx, const T& ry, const T& rz, const T& radians)
    {
        T rotMat[4][4];

        T c = std::cos(radians);
        T s = std::sin(radians);
        T t = 1 - c;

        rotMat[0][0] = t*rx*rx + c;    rotMat[0][1] = t*rx*ry - s*rz; rotMat[0][2] = t*rx*rz + s*ry; rotMat[0][3] = T(0);
        rotMat[1][0] = t*rx*ry + s*rz; rotMat[1][1] = t*ry*ry + c;    rotMat[1][2] = t*ry*rz - s*rx; rotMat[1][3] = T(0);
        rotMat[2][0] = t*rx*rz - s*ry; rotMat[2][1] = t*ry*rz + s*rx; rotMat[2][2] = t*rz*rz + c;    rotMat[2][3] = T(0);
        rotMat[3][0] = T(0);           rotMat[3][1] = T(0);           rotMat[3][2] = T(0);           rotMat[3][3] = T(1);
        return Matrix<4, T>(&rotMat[0][0]);
    }

    template <typename T>
    Matrix<4,T> ortho(const T& left,   const T& right,
                      const T& bottom, const T& top,
                      const T& zNear,  const T& zFar)
    {
        Matrix<4,T> ortho;
        ortho[0][0] = 2 / (right - left);
        ortho[1][1] = 2 / (top   - bottom);
        ortho[2][2] = 2 / (zNear - zFar);

        ortho[0][3] = -(right + left)   / (right - left);
        ortho[1][3] = -(top   + bottom) / (top   - bottom);
        ortho[2][3] = -(zFar  + zNear)  / (zFar  - zNear);
        return ortho;
    }

    template <typename T>
    Matrix<4,T> frustrum(const T& left,   const T& right,
                         const T& bottom, const T& top,
                         const T& zNear,  const T& zFar)
    {
        T zNear2 = 2*zNear;

        Matrix<4,T> frust;
        frust[0][0] = zNear2/(right-left);
        frust[1][1] = zNear2/(top-bottom);
        frust[2][2] = (zFar+zNear)/(zNear-zFar);
        frust[3][3] = 0;

        frust[0][2] = (right+left)/(right-left);
        frust[1][2] = (top+bottom)/(top-bottom);
        frust[3][2] = -1;
        frust[2][3] = -zNear2*zFar/(zFar-zNear);
        return frust;
    }

    template <typename T>
    Matrix<4,T> perspective(const T& fovyRadians, const T& aspect,
                            const T& zNear,       const T& zFar)
    {
        T halfHeight = zNear * atan(fovyRadians/2);
        T halfWidth = aspect * halfHeight;
        return frustrum(-halfWidth,  halfWidth,
                        -halfHeight, halfHeight,
                        zNear,       zFar);
    }

    template <typename T>
    Matrix<4,T> lookAt(const Vector<3, T>& from,
                       const Vector<3, T>& to,
                       const Vector<3, T>& up)
    {
        Vector<3, T> z = (from - to).normalize();
        Vector<3, T> x = cross(up, z).normalize();
        Vector<3, T> y = cross(z, x).normalize();

        T m[4][4];
        m[0][0] = x[0]; m[0][1] = x[1]; m[0][2] = x[2]; m[0][3] = -dot(x, from);
        m[1][0] = y[0]; m[1][1] = y[1]; m[1][2] = y[2]; m[1][3] = -dot(y, from);
        m[2][0] = z[0]; m[2][1] = z[1]; m[2][2] = z[2]; m[2][3] = -dot(z, from);
        m[3][0] = 0;    m[3][1] = 0;    m[3][2] = 0;    m[3][3] = 1;

        return Matrix<4, T>(&m[0][0]);
    }

    template<int N, typename T >
    std::ostream& operator<< (std::ostream& out, const Matrix<N,T>& mat)
    {
        for(int i=0; i<N; ++i)
        {
            out << '[';
            for(int j=0; j<N; ++j)
            {
                out << mat[i][j];
                if(j != N-1)
                    out << ",\t";
            }
            out << "]\n";
        }
        return out;
    }

    template<int N, typename T >
    std::istream& operator>> (std::istream& in, Matrix<N,T>& mat)
    {
        char c;
        for(int i=0; i<N; ++i)
        {
            in >> c;
            for(int j=0; j<N; ++j)
            {
                in >> mat[i][j];
                if(j != N-1)
                    in >> c;
            }
            in >> c;
        }
        return in;
    }
}

#endif // CELLARWORKBENCH_MATRIX_H
