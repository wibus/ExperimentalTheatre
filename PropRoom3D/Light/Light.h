#ifndef PROPROOM3D_LIGHT_H
#define PROPROOM3D_LIGHT_H

#include <GLM/glm.hpp>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{

    struct PROP3D_EXPORT Light
    {
        Light();

        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
    };

    struct PROP3D_EXPORT DirectionnalLight : public Light
    {
        DirectionnalLight();

        glm::vec4 direction;
    };

    struct PROP3D_EXPORT PointLight : public Light
    {
        PointLight();

        glm::vec4 position;
        glm::vec4 attenuationCoefs;
    };

    struct PROP3D_EXPORT SpotLight : public PointLight
    {
        SpotLight();

        glm::vec4 direction;
        float cutoff;
    };


    // Utilities

    // Kevin degree to RGB color value
    // Perfect white is 6600K
    glm::dvec3 kelvinToRgb(int kelvin);
}

#endif // PROPROOM3D_LIGHT_H
