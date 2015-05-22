#ifndef PROPROOM3D_AIR_H
#define PROPROOM3D_AIR_H

#include "Material.h"


namespace prop3
{

    class PROP3D_EXPORT Air :
            public Material
    {
    public:
        Air();
        virtual ~Air();

        virtual void brdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& selfEnteredMaterial,
            unsigned int outRayCountHint)  const override;
    };
}

#endif // PROPROOM3D_AIR_H
