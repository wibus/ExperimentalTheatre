#ifndef PROPROOM3D_NOCOATING_H
#define PROPROOM3D_NOCOATING_H

#include "Coating.h"


namespace prop3
{

    class PROP3D_EXPORT NoCoating :
            public Coating
    {
    public:
        NoCoating();
        virtual ~NoCoating();

        // SceneNode interface
        virtual void accept(SceneVisitor& visitor) override;

        virtual void brdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial,
            unsigned int outRayCountHint) const override;
    };
}

#endif // PROPROOM3D_NOCOATING_H
