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
            const Material& leavedMaterial,
            const Material& enteredMaterial,
            unsigned int outRayCountHint)  const override;

        virtual glm::dvec3 directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const override;

        static const double AIR_REFRACTIVE_INDEX;
    };
}

#endif // PROPROOM3D_AIR_H
