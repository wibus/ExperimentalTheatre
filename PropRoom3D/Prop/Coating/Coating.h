#ifndef PROPROOM3D_COATING_H
#define PROPROOM3D_COATING_H

#include <memory>
#include <vector>

#include <GLM/glm.hpp>

#include "PropRoom3D/StageSet/StageSetNode.h"


namespace prop3
{
    class Raycast;
    class RayHitReport;
    class Material;


    class PROP3D_EXPORT Coating : public StageSetNode
    {
    protected:
        Coating();

    public:
        virtual ~Coating();

        virtual void indirectBrdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const Material& leavedMaterial,
            const Material& enteredMaterial,
            unsigned int outRayCountHint) const = 0;

        virtual glm::dvec3 directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const = 0;
    };
}

#endif // PROPROOM3D_COATING_H
