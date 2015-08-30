#include "NoCoating.h"

#include "../Material/Material.h"
#include "../../StageSet/StageSetVisitor.h"


namespace prop3
{
    NoCoating::NoCoating()
    {
    }

    NoCoating::~NoCoating()
    {

    }

    void NoCoating::indirectBrdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& enteredMaterial,
        unsigned int outRayCountHint) const
    {
        enteredMaterial->indirectBrdf(
            raycasts,
            report,
            leavedMaterial,
            enteredMaterial,
            outRayCountHint);
    }

    glm::dvec3 NoCoating::directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial) const
    {
        return enteredMaterial->directBrdf(
            report,
            outDirection,
            leavedMaterial,
            enteredMaterial);
    }

    void NoCoating::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
