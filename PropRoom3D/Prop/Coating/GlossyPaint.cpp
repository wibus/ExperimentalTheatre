#include "GlossyPaint.h"

#include "../Ray/Raycast.h"
#include "../Ray/RayUtils.h"
#include "../Material/Material.h"


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

    void GlossyPaint::brdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& enteredMaterial,
        unsigned int outRayCountHint) const
    {
        size_t preSize, postSize;

        double glossRatio = computeReflexionRatio(
            leavedMaterial->refractiveIndex(),
            _varnishRefractiveIndex,
            report.ray.direction,
            report.normal);


        // Varnish glossy reflection
        preSize = raycasts.size();
        glossyReflection(raycasts, report, leavedMaterial, _glossiness, outRayCountHint);
        postSize = raycasts.size();

        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= glossRatio;
        }


        // Pigment diffuse reflection
        glm::dvec3 color = _color * glm::max(0.0,
            -glm::dot(report.ray.direction, report.normal));
        color *= (1.0 - glossRatio);

        preSize = raycasts.size();
        diffuseReflection(raycasts, report, leavedMaterial, outRayCountHint);
        postSize = raycasts.size();

        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= color;
        }
    }
}
