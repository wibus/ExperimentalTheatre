#ifndef PROPROOM3D_RAY_H
#define PROPROOM3D_RAY_H

#include <GLM/glm.hpp>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    struct PROP3D_EXPORT Ray
    {
        Ray(const glm::dvec3& origin,
            const glm::dvec3& direction);

        glm::dvec3 origin;
        glm::dvec3 direction;
    };
}

#endif // PROPROOM3D_RAY_H
