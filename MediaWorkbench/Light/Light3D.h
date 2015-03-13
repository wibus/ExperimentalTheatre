#ifndef MEDIAWORKBENCH_LIGHT3D_H
#define MEDIAWORKBENCH_LIGHT3D_H

#include <GLM/glm.hpp>

#include "../libMediaWorkbench_global.h"


namespace media
{
    struct MEDIA_EXPORT Material
    {
        Material();

        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;
        float fresnel;
    };

    struct MEDIA_EXPORT Light3D
    {
        Light3D();

        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
    };

    struct MEDIA_EXPORT DirectionnalLight3D : public Light3D
    {
        DirectionnalLight3D();

        glm::vec4 direction;
    };

    struct MEDIA_EXPORT PointLight3D : public Light3D
    {
        PointLight3D();

        glm::vec4 position;
        glm::vec4 attenuationCoefs;
    };

    struct MEDIA_EXPORT SpotLight3D : public PointLight3D
    {
        SpotLight3D();

        glm::vec4 direction;
        float cutoff;
    };
}

#endif // MEDIAWORKBENCH_LIGHT3D_H
