#include "FlatPaint.h"

#include <glm/gtc/random.hpp>

namespace prop3
{
    FlatPaint::FlatPaint(const glm::vec3& color)
    {
        setMediumColor(color);
    }

    FlatPaint::~FlatPaint()
    {

    }

    glm::dvec3 FlatPaint::computeReflection(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal) const
    {
        glm::dvec3 randDir = glm::sphericalRand(1.0);
        if(glm::dot(randDir, surfaceNormal) < 0.0)
            randDir = -randDir;
        return randDir;
    }

     glm::dvec3 FlatPaint::computeRefraction(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal) const
    {
        return glm::vec3(0.0);
    }

    double FlatPaint::computeReflexionRatio(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal) const
    {
        return 1.0;
    }

    glm::vec3 FlatPaint::computeBrdf(
            const glm::vec3& incomingRadiosity,
            const glm::dvec3& lightDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& viewDirection) const
    {
        return (incomingRadiosity * mediumColor()) *
                glm::max(0.0f, (float)glm::dot(lightDirection, surfaceNormal));
    }
}
