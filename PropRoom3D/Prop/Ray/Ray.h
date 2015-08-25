#ifndef PROPROOM3D_RAY_H
#define PROPROOM3D_RAY_H

#include <GLM/glm.hpp>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    struct PROP3D_EXPORT Ray
    {
        Ray(const glm::dvec3& origin,
            const glm::dvec3& direction,
            double limit = BACKDROP_DISTANCE);

        glm::dvec3 origin;
        glm::dvec3 direction;
        double limit;


        static const double BACKDROP_DISTANCE;
    };
}

#endif // PROPROOM3D_RAY_H
