#include "Metal.h"

#include "Ray/Raycast.h"
#include "Ray/RayUtils.h"
#include "../../StageSet/StageSetVisitor.h"


namespace prop3
{
    Metal::Metal(const glm::dvec3& color, double glossiness) :
        _color(color),
        _glossiness(glossiness)
    {
    }

    Metal::~Metal()
    {

    }

    void Metal::indirectBrdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& selfEnteredMaterial,
        unsigned int outRayCountHint) const
    {
        size_t preSize = raycasts.size();
        indirectGlossyReflection(raycasts, report, leavedMaterial, _glossiness, outRayCountHint);
        size_t postSize = raycasts.size();

        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= _color;
        }
    }

    glm::dvec3 Metal::directBrdf(
        const RayHitReport& report,
        const glm::dvec3& outDirection,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& enteredMaterial) const
    {
        return _color * directGlossyReflection(report, outDirection, _glossiness);
    }

    glm::dvec3 Metal::lightAttenuation(const Raycast& ray) const
    {
        return glm::dvec3(0);
    }

    void Metal::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
