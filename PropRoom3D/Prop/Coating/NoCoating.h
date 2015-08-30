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

        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;

        virtual void indirectBrdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial,
            unsigned int outRayCountHint) const override;

        virtual glm::dvec3 directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial) const override;
    };
}

#endif // PROPROOM3D_NOCOATING_H
