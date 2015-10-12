#ifndef PROPROOM3D_RAYCAST_H
#define PROPROOM3D_RAYCAST_H

#include <memory>

#include <GLM/glm.hpp>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class Material;


    struct PROP3D_EXPORT Raycast
    {
        Raycast(double limit,
                double entropy,
                const glm::dvec3& color,
                const glm::dvec3& origin,
                const glm::dvec3& direction,
                const std::shared_ptr<Material>& material);

        double limit;
        double entropy;
        glm::dvec3 color;
        glm::dvec3 origin;
        glm::dvec3 direction;
        std::shared_ptr<Material> material;

        static const double BACKDROP_DISTANCE;
        static const double FULLY_SPECULAR_ENTROPY;
        static const double FULLY_DIFFUSIVE_ENTROPY;

        static double mixEntropies(double inEntropy, double outEntropy);
        static double compatibility(double inEntropy, double outEntropy);
    };
}

#endif // PROPROOM3D_RAYCAST_H
