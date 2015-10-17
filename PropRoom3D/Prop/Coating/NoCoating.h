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
            const Material& leavedMaterial,
            const Material& enteredMaterial,
            unsigned int outRayCountHint) const override;

        virtual glm::dvec3 directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const override;
    };
}

#endif // PROPROOM3D_NOCOATING_H
