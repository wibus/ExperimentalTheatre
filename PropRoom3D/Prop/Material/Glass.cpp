#include "Glass.h"

#include "../Ray/Raycast.h"
#include "../Ray/RayUtils.h"
#include "../../Scene/SceneVisitor.h"


namespace prop3
{
    Glass::Glass(const glm::dvec3& color,
                 double concentration,
                 double refractiveIndex) :
        _color(color),
        _concentration(concentration)
    {
        setRefractiveIndex(refractiveIndex);
    }

    Glass::~Glass()
    {

    }

    void Glass::brdf(
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

    glm::dvec3 Glass::lightAttenuation(
                const Ray& ray,
                double distance) const
    {
        if(_concentration == 0.0)
            return glm::dvec3(1.0);

        return glm::pow(_color, glm::dvec3(_concentration * distance));
    }

    void Glass::accept(SceneVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
