#include "Chrome.h"


namespace prop3
{
    Chrome::Chrome(const glm::dvec3& color)
    {
        setMediumColor(color);
    }

    Chrome::~Chrome()
    {

    }

    glm::dvec3 Chrome::computeReflection(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& texCoord) const
    {
        return glm::reflect(incidentDirection, surfaceNormal);
    }

    glm::dvec3 Chrome::computeReflectionBrdf(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& viewDirection,
            const glm::dvec3& texCoord) const
    {
        return mediumColor();
    }
}
