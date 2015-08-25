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

    void NoCoating::brdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& enteredMaterial,
        unsigned int outRayCountHint) const
    {
        enteredMaterial->brdf(
            raycasts,
            report,
            leavedMaterial,
            enteredMaterial,
            outRayCountHint);
    }

    void NoCoating::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
