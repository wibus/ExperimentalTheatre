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
        const Material& leavedMaterial,
        const Material& enteredMaterial,
        unsigned int outRayCountHint) const
    {
        indirectSpecularRefraction(
            raycasts,
            report,
            leavedMaterial.refractiveIndex(),
            enteredMaterial.refractiveIndex());
    }

    glm::dvec3 Air::directBrdf(
        const RayHitReport& report,
        const glm::dvec3& outDirection,
        const Material& leavedMaterial,
        const Material& enteredMaterial) const
    {
        return directSpecularRefraction(report, outDirection);
    }

    void Air::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
