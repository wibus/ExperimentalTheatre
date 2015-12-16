#include "Raycast.h"


namespace prop3
{
    const double Raycast::BACKDROP_LIMIT = INFINITY;
    const double Raycast::INITIAL_DISTANCE = 0.0;

    Raycast::Raycast(
            double limit,
            double pathLength,
            const glm::dvec4& sample,
            const glm::dvec3& origin,
            const glm::dvec3& direction) :
        limit(limit),
        pathLength(pathLength),
        sample(sample),
        origin(origin),
        direction(direction)
    {}
}
