#include "Air.h"

#include "Ray/Raycast.h"
#include "Ray/RayUtils.h"
#include "../../StageSet/StageSetVisitor.h"


namespace prop3
{
    const double Air::AIR_REFRACTIVE_INDEX = 1.0002772;

    Air::Air(double refractiveIndex)
    {
        setRefractiveIndex(refractiveIndex);
    }

    Air::~Air()
    {

    }

    void Air::indirectBrdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& selfEnteredMaterial,
        unsigned int outRayCountHint) const
    {
        indirectSpecularRefraction(
            raycasts,
            report,
            leavedMaterial,
            selfEnteredMaterial);
    }

    glm::dvec3 Air::directBrdf(
        const RayHitReport& report,
        const glm::dvec3& outDirection,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& enteredMaterial) const
    {
        return directSpecularRefraction(report, outDirection);
    }

    void Air::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
