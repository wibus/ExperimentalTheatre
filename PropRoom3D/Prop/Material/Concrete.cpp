#include "Concrete.h"

#include "../Ray/Raycast.h"
#include "../Ray/RayUtils.h"
#include "../../StageSet/StageSetVisitor.h"


namespace prop3
{
    Concrete::Concrete(const glm::dvec3& color) :
        _color(color)
    {
    }

    Concrete::~Concrete()
    {

    }

    void Concrete::indirectBrdf(std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& selfEnteredMaterial,
        unsigned int outRayCountHint) const
    {
        size_t preSize = raycasts.size();
        indirectDiffuseReflection(raycasts, report, selfEnteredMaterial, outRayCountHint);
        size_t postSize = raycasts.size();

        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= _color * glm::max(0.0,
                glm::dot(raycasts[i].ray.direction, report.normal));
        }
    }

    glm::dvec3 Concrete::directBrdf(
        const RayHitReport& report,
        const glm::dvec3& outDirection,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& enteredMaterial) const
    {
        return _color * directDiffuseReflection(report, outDirection);
    }

    void Concrete::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
