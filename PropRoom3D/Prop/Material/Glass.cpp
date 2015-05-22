#include "Glass.h"

#include "../Ray/Raycast.h"
#include "../Ray/RayUtils.h"


namespace prop3
{
    Glass::Glass(const glm::dvec3& color, double dyeConcentration) :
        _color(color),
        _dyeConcentration(dyeConcentration)
    {
        setRefractiveIndex(1.5);
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
        if(_dyeConcentration == 0.0)
            return glm::dvec3(1.0);

        double alpha = 1.0 / (1.0 + distance*_dyeConcentration);
        alpha *= alpha *= alpha;

        return glm::mix(_color, glm::dvec3(1.0), alpha);
    }
}
