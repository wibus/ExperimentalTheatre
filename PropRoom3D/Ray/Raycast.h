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
                double pathLength,
                const glm::dvec4& sample,
                const glm::dvec3& origin,
                const glm::dvec3& direction);

        double limit;
        double pathLength;
        glm::dvec4 sample;
        glm::dvec3 origin;
        glm::dvec3 direction;

        static const double BACKDROP_LIMIT;
        static const double INITIAL_DISTANCE;
    };
}

#endif // PROPROOM3D_RAYCAST_H
