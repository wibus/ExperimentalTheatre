#include "TexturedGlossyPaint.h"

#include <GLM/gtc/random.hpp>

#include <CellarWorkbench/Image/Image.h>
#include <CellarWorkbench/Image/ImageBank.h>

#include "Ray/Raycast.h"
#include "Ray/RayUtils.h"
#include "../Material/Material.h"
#include "../../StageSet/StageSetVisitor.h"


namespace prop3
{
    TexturedGlossyPaint::TexturedGlossyPaint(
            const std::string& texName,
            const std::string& glossName,
            const cellar::ESamplerFilter& texFilter,
            const cellar::ESamplerWrapper& texWrapper,
            const glm::dvec3& defaultColor,
            double defaultGlossiness,
            double varnishRefractiveIndex) :
        _texName(texName),
        _glossName(glossName),
        _texture(cellar::getImageBank().getImage(texName)),
        _glossMap(cellar::getImageBank().getImage(glossName)),
        _sampler(texFilter, texWrapper),
        _defaultColor(defaultColor),
        _defaultGlossiness(defaultGlossiness),
        _varnishRefractiveIndex(varnishRefractiveIndex)
    {
    }

    TexturedGlossyPaint::~TexturedGlossyPaint()
    {

    }

    void TexturedGlossyPaint::indirectBrdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial,
            unsigned int outRayCountHint) const
    {
        size_t preSize, postSize;

        double mirrorRatio = computeReflexionRatio(
            leavedMaterial->refractiveIndex(),
            _varnishRefractiveIndex,
            report.incidentRay.direction,
            report.normal);


        // Varnish glossy reflection
        double glossiness = _defaultGlossiness;
        if(report.isTextured)
        {
            const glm::dvec3& texCoord = report.texCoord;
            glossiness = _sampler.sample(
                _glossMap, texCoord.s, texCoord.t).r;
            mirrorRatio *= glossiness;
        }

        preSize = raycasts.size();
        indirectGlossyReflection(raycasts, report, leavedMaterial, glossiness, outRayCountHint);
        postSize = raycasts.size();

        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= mirrorRatio;
        }


        // Pigment diffuse reflection
        preSize = raycasts.size();
        indirectDiffuseReflection(raycasts, report, leavedMaterial, outRayCountHint);
        postSize = raycasts.size();

        glm::dvec3 color = _defaultColor;
        if(report.isTextured)
        {
            const glm::dvec3& texCoord = report.texCoord;
            color = glm::dvec3(_sampler.sample(
                _texture, texCoord.x, texCoord.y));
        }

        color *= (1.0 - mirrorRatio);
        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= color;
        }
    }

    glm::dvec3 TexturedGlossyPaint::directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial) const
    {
        double mirrorRatio = computeReflexionRatio(
            leavedMaterial->refractiveIndex(),
            _varnishRefractiveIndex,
            report.incidentRay.direction,
            report.normal);

        double glossiness = _defaultGlossiness;
        if(report.isTextured)
        {
            const glm::dvec3& texCoord = report.texCoord;
            int i = texCoord.s * _glossMap.width();
            int j = texCoord.t * _glossMap.height();
            unsigned char* pixel = _glossMap.pixel(
                glm::clamp(i, 0,  _glossMap.width()-1),
                glm::clamp(j, 0, _glossMap.height()-1));

            // Not blended with default color
            glossiness = pixel[0] / 255.0;
            mirrorRatio *= glossiness;
        }

        glm::dvec3 color = _defaultColor;
        if(report.isTextured)
        {
            const glm::dvec3& texCoord = report.texCoord;
            color = glm::dvec3(_sampler.sample(
                _texture, texCoord.x, texCoord.y));
        }


        return glm::mix(
            color * directDiffuseReflection(report, outDirection),
            directGlossyReflection(report, outDirection, glossiness),
            mirrorRatio);
    }

    void TexturedGlossyPaint::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
