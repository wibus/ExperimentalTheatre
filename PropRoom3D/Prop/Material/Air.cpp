#include "Air.h"

#include "../Ray/Raycast.h"
#include "../Ray/RayUtils.h"
#include "../../Scene/SceneVisitor.h"


namespace prop3
{
    Air::Air(double refractiveIndex)
    {
        setRefractiveIndex(refractiveIndex);
    }

    Air::~Air()
    {

    }

    void Air::brdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& selfEnteredMaterial,
        unsigned int outRayCountHint) const
    {
        specularRefraction(
            raycasts,
            report,
            leavedMaterial,
            selfEnteredMaterial);
    }

    void Air::accept(SceneVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
