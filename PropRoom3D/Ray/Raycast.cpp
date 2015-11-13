#include "Raycast.h"


namespace prop3
{
    const double Raycast::BACKDROP_DISTANCE = INFINITY;
    const double Raycast::FULLY_SPECULAR_ENTROPY = 0.0;
    const double Raycast::FULLY_DIFFUSIVE_ENTROPY = 1.0;
    const double Raycast::COMPLETE_RAY_WEIGHT = 1.0;

    Raycast::Raycast(
            double limit,
            double weight,
            double entropy,
            const glm::dvec3& color,
            const glm::dvec3& origin,
            const glm::dvec3& direction) :
        limit(limit),
        weight(weight),
        entropy(entropy),
        color(color),
        origin(origin),
        direction(direction)
    {}

    double Raycast::mixEntropies(double inEntropy, double outEntropy)
    {
        return 1.0 - (1.0 - inEntropy) * (1.0 - outEntropy);
    }

    double Raycast::compatibility(double inEntropy, double outEntropy)
    {
        if(inEntropy == outEntropy)
            return 1.0;

        // denum can't be null since
        //  * entropy is in [0, 1]
        //  * (in == 0) & (out == 0)  <->  denum == 0
        //    (in == 0) & (out == 0)  ->   inEntropy == outEntropy
        //      which is handled by the first check

        double num = glm::abs(inEntropy - outEntropy);
        double denum = inEntropy + outEntropy;
        return 1.0 - (num / denum);
    }
}
