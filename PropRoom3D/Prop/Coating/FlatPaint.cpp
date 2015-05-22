#include "FlatPaint.h"

#include "../Ray/Raycast.h"
#include "../Ray/RayUtils.h"


namespace prop3
{
    FlatPaint::FlatPaint(const glm::dvec3& color) :
        _color(color)
    {
    }

    FlatPaint::~FlatPaint()
    {

    }

    void FlatPaint::brdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& enteredMaterial,
        unsigned int outRayCountHint) const
    {
        // Pigment diffuse reflection
        glm::dvec3 color = _color * glm::max(0.0,
            -glm::dot(report.ray.direction, report.normal));

        size_t preSize = raycasts.size();
        diffuseReflection(raycasts, report, leavedMaterial, outRayCountHint);
        size_t postSize = raycasts.size();

        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= color;
        }
    }
}
