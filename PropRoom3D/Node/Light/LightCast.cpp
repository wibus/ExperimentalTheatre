#include "LightCast.h"

namespace prop3
{
    LightCast::LightCast(
            const Raycast& raycast,
            const glm::dvec3& emittingPosition,
            const glm::dvec3& emittingDirection,
            DiffuseSize diffuseSize) :
        raycast(raycast),
        emittingPosition(emittingPosition),
        emittingDirection(emittingDirection),
        diffuseSize(diffuseSize)
    {

    }
}
