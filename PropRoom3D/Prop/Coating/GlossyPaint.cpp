#include "GlossyPaint.h"

#include "Ray/Raycast.h"
#include "Ray/RayUtils.h"
#include "../Material/Material.h"
#include "../../StageSet/StageSetVisitor.h"


namespace prop3
{
    GlossyPaint::GlossyPaint(const glm::dvec3& color,
                             double glossiness,
                             double varnishRefractiveIndex) :
        _color(color),
        _glossiness(glossiness),
        _varnishRefractiveIndex(varnishRefractiveIndex)
    {
    }

    GlossyPaint::~GlossyPaint()
    {

    }

    void GlossyPaint::indirectBrdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const Material& leavedMaterial,
        const Material& enteredMaterial,
        unsigned int outRayCountHint) const
    {
        size_t preSize, postSize;

        double mirrorRatio = computeReflexionRatio(
            leavedMaterial.refractiveIndex(),
            _varnishRefractiveIndex,
            report.incidentRay.direction,
            report.normal);


        // Varnish glossy reflection
        preSize = raycasts.size();
        indirectGlossyReflection(raycasts, report, _glossiness, outRayCountHint);
        postSize = raycasts.size();

        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= mirrorRatio;
        }


        // Pigment diffuse reflection
        preSize = raycasts.size();
        indirectDiffuseReflection(raycasts, report, outRayCountHint);
        postSize = raycasts.size();

        glm::dvec3 color = _color * (1.0 - mirrorRatio);
        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= color;
        }
    }

    glm::dvec3 GlossyPaint::directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const
    {
        double mirrorRatio = computeReflexionRatio(
            leavedMaterial.refractiveIndex(),
            _varnishRefractiveIndex,
            report.incidentRay.direction,
            report.normal);

        return glm::mix(
            _color * directDiffuseReflection(report, outDirection),
            directGlossyReflection(report, outDirection, _glossiness),
            mirrorRatio);
    }

    void GlossyPaint::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
