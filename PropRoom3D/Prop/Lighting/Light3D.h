#ifndef PROPROOM3D_LIGHT3D_H
#define PROPROOM3D_LIGHT3D_H

#include <GLM/glm.hpp>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    struct PROP3D_EXPORT Material
    {
        Material();

        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;
        float fresnel;
    };

    struct PROP3D_EXPORT Light3D
    {
        Light3D();

        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
    };

    struct PROP3D_EXPORT DirectionnalLight3D : public Light3D
    {
        DirectionnalLight3D();

        glm::vec4 direction;
    };

    struct PROP3D_EXPORT PointLight3D : public Light3D
    {
        PointLight3D();

        glm::vec4 position;
        glm::vec4 attenuationCoefs;
    };

    struct PROP3D_EXPORT SpotLight3D : public PointLight3D
    {
        SpotLight3D();

        glm::vec4 direction;
        float cutoff;
    };
}

#endif // PROPROOM3D_LIGHT3D_H
