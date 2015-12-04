#include "Raycast.h"


namespace prop3
{
    const double Raycast::BACKDROP_DISTANCE = INFINITY;

    Raycast::Raycast(double limit,
            const glm::dvec4& sample,
            const glm::dvec3& origin,
            const glm::dvec3& direction) :
        limit(limit),
        sample(sample),
        origin(origin),
        direction(direction)
    {}
}
