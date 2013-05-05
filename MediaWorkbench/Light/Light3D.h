#ifndef MEDIAWORKBENCH_LIGHT3D_H
#define MEDIAWORKBENCH_LIGHT3D_H

#include "libMediaWorkbench_global.h"
#include "DataStructure/Vector.h"


namespace media
{
    struct MEDIA_EXPORT Material
    {
        Material();

        cellar::Vec4f diffuse;
        cellar::Vec4f specular;
        float shininess;
        float fresnel;
    };

    struct MEDIA_EXPORT Light3D
    {
        Light3D();

        cellar::Vec4f ambient;
        cellar::Vec4f diffuse;
        cellar::Vec4f specular;
    };

    struct MEDIA_EXPORT DirectionnalLight3D : public Light3D
    {
        DirectionnalLight3D();

        cellar::Vec4f direction;
    };

    struct MEDIA_EXPORT PointLight3D : public Light3D
    {
        PointLight3D();

        cellar::Vec4f position;
        cellar::Vec4f attenuationCoefs;
    };

    struct MEDIA_EXPORT SpotLight3D : public PointLight3D
    {
        SpotLight3D();

        cellar::Vec4f direction;
        float cutoff;
    };
}

#endif // MEDIAWORKBENCH_LIGHT3D_H
