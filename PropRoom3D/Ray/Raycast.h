#ifndef PROPROOM3D_RAYCAST_H
#define PROPROOM3D_RAYCAST_H

#include <memory>

#include <GLM/glm.hpp>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class Material;


    class PROP3D_EXPORT Raycast
    {
	public:
        Raycast(double limit,
                double weight,
                double entropy,
                const glm::dvec3& color,
                const glm::dvec3& origin,
                const glm::dvec3& direction);

        double limit;
        double weight;
        double entropy;
        glm::dvec3 color;
        glm::dvec3 origin;
        glm::dvec3 direction;

        static const double BACKDROP_DISTANCE;
        static const double FULLY_SPECULAR_ENTROPY;
        static const double FULLY_DIFFUSIVE_ENTROPY;
        static const double COMPLETE_RAY_WEIGHT;

        static double mixEntropies(double inEntropy, double outEntropy);
        static double compatibility(double inEntropy, double outEntropy);
    };
}

#endif // PROPROOM3D_RAYCAST_H
