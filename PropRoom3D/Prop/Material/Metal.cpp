#include "Metal.h"

#include "../Ray/Raycast.h"
#include "../Ray/RayUtils.h"


namespace prop3
{
    Metal::Metal(const glm::dvec3& color, double glossiness) :
        _color(color),
        _glossiness(glossiness)
    {
    }

    Metal::~Metal()
    {

    }

    void Metal::brdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& selfEnteredMaterial,
        unsigned int outRayCountHint) const
    {
        size_t preSize = raycasts.size();
        glossyReflection(raycasts, report, selfEnteredMaterial, _glossiness, outRayCountHint);
        size_t postSize = raycasts.size();

        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= _color;
        }
    }
}
