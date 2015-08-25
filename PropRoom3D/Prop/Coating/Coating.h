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

        virtual void brdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial,
            unsigned int outRayCountHint) const = 0;
    };



    // IMPLEMENTATION //
}

#endif // PROPROOM3D_COATING_H
