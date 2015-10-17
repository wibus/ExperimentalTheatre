#include "FlatPaint.h"

#include "Ray/Raycast.h"
#include "Ray/RayUtils.h"
#include "../../StageSet/StageSetVisitor.h"


namespace prop3
{
    FlatPaint::FlatPaint(const glm::dvec3& color) :
        _color(color)
    {
    }

    FlatPaint::~FlatPaint()
    {

    }

    void FlatPaint::indirectBrdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const Material& leavedMaterial,
        const Material& enteredMaterial,
        unsigned int outRayCountHint) const
    {
        // Pigment diffuse reflection
        size_t preSize = raycasts.size();
        indirectDiffuseReflection(raycasts, report, outRayCountHint);
        size_t postSize = raycasts.size();

        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= _color;
        }
    }

    glm::dvec3 FlatPaint::directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const
    {
        return _color * directDiffuseReflection(report, outDirection);
    }

    void FlatPaint::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
