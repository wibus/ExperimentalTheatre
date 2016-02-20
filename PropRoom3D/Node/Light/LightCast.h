#ifndef PROPROOM3D_LIGHTCAST_H
#define PROPROOM3D_LIGHTCAST_H

#include <functional>

#include <GLM/glm.hpp>

#include <PropRoom3D/Ray/Raycast.h>


namespace prop3
{
    class PROP3D_EXPORT LightCast
    {
    public:
        typedef std::function<double (const LightCast&, const Raycast&, double)> DiffuseSize;

        LightCast(const Raycast& raycast,
                  const glm::dvec3& emittingPosition,
                  const glm::dvec3& emittingDirection,
                  DiffuseSize diffuseSize);

        Raycast raycast;
        glm::dvec3 emittingPosition;
        glm::dvec3 emittingDirection;
        DiffuseSize diffuseSize;
    };
}

#endif // PROPROOM3D_LIGHTCAST_H
