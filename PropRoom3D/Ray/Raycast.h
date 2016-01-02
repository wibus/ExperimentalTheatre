#ifndef PROPROOM3D_RAYCAST_H
#define PROPROOM3D_RAYCAST_H

#include <memory>

#include <GLM/glm.hpp>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class PROP3D_EXPORT Raycast
    {
	public:
        Raycast(double entropy,
                const glm::dvec4& sample,
                const glm::dvec3& origin,
                const glm::dvec3& direction);

        static double getEntropy(double roughness);
        static double mixEntropies(double e1, double e2);

        static double totalEntropy(
                const Raycast& eyeRay,
                const Raycast& lightRay,
                double roughness);
        static double totalDiffuseDist(
                const Raycast& eyeRay,
                const Raycast& lightRay,
                double roughness);

        double limit;
        double entropy;
        double virtDist;
        double pathLength;
        glm::dvec4 sample;
        glm::dvec3 origin;
        glm::dvec3 direction;

        static const double FULLY_DIFFUSE;
        static const double FULLY_SPECULAR;
        static const double BACKDROP_LIMIT;
        static const double INITIAL_DISTANCE;
    };
}

#endif // PROPROOM3D_RAYCAST_H
