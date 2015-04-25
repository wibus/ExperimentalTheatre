#include "FlatPaint.h"

#include <GLM/gtc/random.hpp>


namespace prop3
{
    FlatPaint::FlatPaint(const glm::dvec3& color)
    {
        setMediumColor(color);
    }

    FlatPaint::~FlatPaint()
    {

    }

    std::shared_ptr<Costume> FlatPaint::clone() const
    {
        return std::shared_ptr<Costume>(new FlatPaint(*this));
    }

    glm::dvec3 FlatPaint::computeReflectionDirection(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& texCoord) const
    {
        glm::dvec3 randDir = glm::sphericalRand(1.0);
        if(glm::dot(randDir, surfaceNormal) < 0.0)
            randDir = -randDir;
        return randDir;
    }

    glm::dvec3 FlatPaint::computeReflectionBrdf(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& viewDirection,
            const glm::dvec3& texCoord) const
    {
        return mediumColor() * glm::max(0.0,
            -glm::dot(incidentDirection, surfaceNormal));
    }
}
