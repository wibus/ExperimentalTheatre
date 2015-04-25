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

    std::shared_ptr<Costume> Chrome::clone() const
    {
        return std::shared_ptr<Costume>(new Chrome(*this));
    }

    glm::dvec3 Chrome::computeReflectionDirection(
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
