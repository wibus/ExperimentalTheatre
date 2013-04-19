#ifndef LIGHT3D_H
#define LIGHT3D_H

#include "libCellarWorkbench_global.h"
#include "DataStructure/Vector.h"


namespace cellar
{
    struct CELLAR_EXPORT Material
    {
        Material();

        Vec4f diffuse;
        Vec4f specular;
        float shininess;
        float fresnel;
    };

    struct CELLAR_EXPORT Light3D
    {
        Light3D();

        Vec4f ambient;
        Vec4f diffuse;
        Vec4f specular;
    };

    struct CELLAR_EXPORT DirectionnalLight3D : public Light3D
    {
        DirectionnalLight3D();

        Vec4f direction;
    };

    struct CELLAR_EXPORT PointLight3D : public Light3D
    {
        PointLight3D();

        Vec4f position;
        Vec4f attenuationCoefs;
    };

    struct CELLAR_EXPORT SpotLight3D : public PointLight3D
    {
        SpotLight3D();

        Vec4f direction;
        float cutoff;
    };
}

#endif // LIGHT3D_H
