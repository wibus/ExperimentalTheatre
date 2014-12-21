#ifndef PROPROOM3D_RAYCAST_H
#define PROPROOM3D_RAYCAST_H

#include <cmath>

#include <map>

#include <GLM/glm.hpp>

#include "libPropRoom3D_global.h"


namespace prop3
{
    struct PROP3D_EXPORT Ray
    {
        Ray(const glm::dvec3& origin,
            const glm::dvec3& direction) :
            origin(origin),
            direction(direction)
        {}

        glm::dvec3 origin;
        glm::dvec3 direction;
    };


    struct PROP3D_EXPORT RaycastReport
    {
        RaycastReport(double t,
                      const glm::dvec3& position,
                      const glm::dvec3& normal,
                      const glm::dvec3& texCoord = glm::dvec3()) :
            t(t),
            position(position),
            normal(normal),
            texCoord(texCoord)
        {}

        double t;
        glm::dvec3 position;
        glm::dvec3 normal;
        glm::dvec3 texCoord;
    };
}

#endif // PROPROOM3D_RAYCAST_H
