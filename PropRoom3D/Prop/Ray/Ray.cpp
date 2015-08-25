#include "Ray.h"


namespace prop3
{
    const double Ray::BACKDROP_DISTANCE = INFINITY;


    Ray::Ray(
            const glm::dvec3& origin,
            const glm::dvec3& direction,
            double limit) :
        origin(origin),
        direction(direction),
        limit(limit)
    {

    }
}
