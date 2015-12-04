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
                const glm::dvec4& sample,
                const glm::dvec3& origin,
                const glm::dvec3& direction);

        double limit;
        glm::dvec4 sample;
        glm::dvec3 origin;
        glm::dvec3 direction;

        static const double BACKDROP_DISTANCE;
    };
}

#endif // PROPROOM3D_RAYCAST_H
