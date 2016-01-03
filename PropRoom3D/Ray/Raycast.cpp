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
        return glm::pow(roughness, 1/4.0);
    }

    double Raycast::mixEntropies(double e1, double e2)
    {
        return 1.0 - (1.0 - e1) * (1.0 - e2);
    }

    double Raycast::totalEntropy(
            const Raycast& eyeRay,
            const Raycast& lightRay,
            double roughness)
    {
        return 1.0 - (1.0 - eyeRay.entropy) *
                (1.0 - getEntropy(roughness)) *
                (1.0 - lightRay.entropy);
    }

    double Raycast::totalDiffuseDist(
            const Raycast& eyeRay,
            const Raycast& lightRay,
            double roughness)
    {
        double specularity = (1.0 - eyeRay.entropy) * (1.0 - getEntropy(roughness));
        double endDist = (lightRay.pathLength - lightRay.virtDist * specularity);
        double bindingDist = lightRay.limit * (1.0 - specularity);
        return eyeRay.virtDist + bindingDist + endDist;
    }
}
