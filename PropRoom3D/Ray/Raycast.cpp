#include "Raycast.h"


namespace prop3
{
    const double Raycast::FULLY_DIFFUSE = 1.0;
    const double Raycast::FULLY_SPECULAR = 0.0;
    const double Raycast::BACKDROP_LIMIT = INFINITY;
    const double Raycast::INITIAL_DISTANCE = 0.0;

    Raycast::Raycast(
            double entropy,
            const glm::dvec4& sample,
            const glm::dvec3& origin,
            const glm::dvec3& direction) :
        limit(BACKDROP_LIMIT),
        entropy(entropy),
        virtDist(INITIAL_DISTANCE),
        pathLength(INITIAL_DISTANCE),
        sample(sample),
        origin(origin),
        direction(direction)
    {}

    double Raycast::getEntropy(double roughness)
    {
        return roughness;
    }

    double Raycast::mixEntropies(double entropy1, double entropy2)
    {
        return 1.0 - (1.0 - entropy1) * (1.0 - entropy2);
    }
}
