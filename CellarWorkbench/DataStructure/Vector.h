#ifndef CELLARWORKBENCH_VECTOR_H
#define CELLARWORKBENCH_VECTOR_H

#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "VectorSpace.h"


namespace cellar
{
    template <typename T>
    Vector2D<T> vec2(const Vector3D<T>& vec);
    template <typename T>
    Vector2D<T> vec2(const Vector4D<T>& vec);

    template <typename T>
    Vector3D<T> vec3(const Vector2D<T>& vec, const T& z=0);
    template <typename T>
    Vector3D<T> vec3(const Vector4D<T>& vec);

    template <typename T>
    Vector4D<T> vec4(const Vector2D<T>& vec, const T& z=0, const T& w=1);
    template <typename T>
    Vector4D<T> vec4(const Vector3D<T>& vec, const T& w=1);



    // IMPLEMENTATION //
    template <typename T>
    inline Vector2D<T> vec2(const Vector3D<T>& vec)
    {
        return Vector2D<T>(vec.x(), vec.y());
    }

    template <typename T>
    inline Vector2D<T> vec2(const Vector4D<T>& vec)
    {
        return Vector2D<T>(vec.x(), vec.y());
    }


    template <typename T>
    inline Vector3D<T> vec3(const Vector2D<T>& vec, const T& z)
    {
        return Vector3D<T>(vec.x(), vec.y(), z);
    }

    template <typename T>
    inline Vector3D<T> vec3(const Vector4D<T>& vec)
    {
        return Vector3D<T>(vec.x(), vec.y(), vec.z());
    }


    template <typename T>
    inline Vector4D<T> vec4(const Vector2D<T>& vec, const T& z, const T& w)
    {
        return Vector4D<T>(vec.x(), vec.y(), z, w);
    }

    template <typename T>
    inline Vector4D<T> vec4(const Vector3D<T>& vec, const T& w)
    {
        return Vector4D<T>(vec.x(), vec.y(), vec.z(), w);
    }
}

#endif // CELLARWORKBENCH_VECTOR_H
