#ifndef LIGHT3D_H
#define LIGHT3D_H

#include "libCellarWorkbench_global.h"
#include "DataStructure/Vector.h"


namespace cellar
{
    struct CELLAR_EXPORT Material
    {
        Material() :
            diffuse(0.5f, 0.5f, 0.5f, 1.0f),
            specular(0.1f, 0.1f, 0.1f, 1.0f),
            shininess(64),
            fresnel(0.05)
        {}

        Vec4f diffuse;
        Vec4f specular;
        float shininess;
        float fresnel;
    };

    struct CELLAR_EXPORT Light3D
    {
        Light3D() :
            ambient( 0.2, 0.2, 0.2, 1.0),
            diffuse( 0.8, 0.8, 0.8, 1.0),
            specular(0.4, 0.4, 0.4, 1.0)
        {}

        Vec4f ambient;
        Vec4f diffuse;
        Vec4f specular;
    };

    struct CELLAR_EXPORT DirectionnalLight3D : public Light3D
    {
        DirectionnalLight3D() :
            direction(0.0, 0.0, -1.0, 0.0)
        {}

        Vec4f direction;
    };

    struct CELLAR_EXPORT PointLight3D : public Light3D
    {
        PointLight3D() :
            position(0.0, 0.0, 0.0, 1.0),
            attenuationCoefs(1.0, 0.0, 0.0)
        {}

        Vec4f position;
        Vec4f attenuationCoefs;
    };

    struct CELLAR_EXPORT SpotLight3D : public PointLight3D
    {
        SpotLight3D() :
            direction(1, 0, 0),
            cutoff(1.0f)
        {
        }

        Vec4f direction;
        float cutoff;
    };
}

#endif // LIGHT3D_H
