#include "Concrete.h"

#include "../Ray/Raycast.h"
#include "../Ray/RayUtils.h"
#include "../../Scene/SceneVisitor.h"


namespace prop3
{
    Concrete::Concrete(const glm::dvec3& color) :
        _color(color)
    {
    }

    Concrete::~Concrete()
    {

    }

    void Concrete::brdf(std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& selfEnteredMaterial,
        unsigned int outRayCountHint) const
    {
        size_t preSize = raycasts.size();
        diffuseReflection(raycasts, report, selfEnteredMaterial, outRayCountHint);
        size_t postSize = raycasts.size();

        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= _color * glm::max(0.0,
                glm::dot(raycasts[i].ray.direction, report.normal));
        }
    }

    void Concrete::accept(SceneVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
