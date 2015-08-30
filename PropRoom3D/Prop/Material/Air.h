#ifndef PROPROOM3D_AIR_H
#define PROPROOM3D_AIR_H

#include "Material.h"


namespace prop3
{

    class PROP3D_EXPORT Air :
            public Material
    {
    public:
        Air(double refractiveIndex = AIR_REFRACTIVE_INDEX);
        virtual ~Air();

        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;


        virtual void indirectBrdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& selfEnteredMaterial,
            unsigned int outRayCountHint)  const override;

        virtual glm::dvec3 directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial) const override;

        static const double AIR_REFRACTIVE_INDEX;
    };
}

#endif // PROPROOM3D_AIR_H
