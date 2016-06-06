#include "TexturedStdCoating.h"

#include <CellarWorkbench/Image/Image.h>
#include <CellarWorkbench/Image/ImageBank.h>

#include "Ray/RayHitReport.h"
#include "Node/Visitor.h"


namespace prop3
{
    const std::string TexturedStdCoating::NO_TEXTURE = "NO_TEXTURE";

    TexturedStdCoating::TexturedStdCoating() :
        _defaultRoughness(0.0),
        _roughnessTexName(NO_TEXTURE),
        _defaultPaintColor(1.0),
        _paintColorTexName(NO_TEXTURE),
        _paintRefractiveIndex(1.55),
        _sampler(cellar::ESamplerFilter::NEAREST,
                 cellar::ESamplerWrapper::CLAMP),
        _roughnessTexture(nullptr),
        _paintColorTexture(nullptr)
    {
    }

    TexturedStdCoating::~TexturedStdCoating()
    {

    }

    void TexturedStdCoating::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    double TexturedStdCoating::roughness(const glm::dvec3& tex) const
    {
        if(tex == RayHitReport::NO_TEXCOORD || _roughnessTexName == NO_TEXTURE)
        {
            return _defaultRoughness;
        }

        return 1.0 - _sampler.sample(*_roughnessTexture, tex.s, tex.t).r;
    }

    void TexturedStdCoating::setDefaultRoughness(double roughness)
    {
        _defaultRoughness = roughness;
    }

    void TexturedStdCoating::setRoughnessTexName(const std::string& name)
    {
        _roughnessTexName = name;

        if(name == NO_TEXTURE)
            _roughnessTexture.reset();
        else
            _roughnessTexture = cellar::getImageBank().getImagePtr(name);
    }

    glm::dvec4 TexturedStdCoating::paintColor(const glm::dvec3& tex) const
    {
        if(tex == RayHitReport::NO_TEXCOORD || _paintColorTexName == NO_TEXTURE)
        {
            return _defaultPaintColor;
        }

        return glm::dvec4(glm::dvec3(_sampler.sample(*_paintColorTexture, tex.s, tex.t)), 1.0);
    }

    void TexturedStdCoating::setDefaultPaintColor(const glm::dvec4& color)
    {
        _defaultPaintColor = color;
    }

    void TexturedStdCoating::setPaintColorTexName(const std::string& name)
    {
        _paintColorTexName = name;

        if(name == NO_TEXTURE)
            _paintColorTexture.reset();
        else
        {
            std::string linearizedName = name + "-linearized";
            if(cellar::getImageBank().isInBank(linearizedName))
                _paintColorTexture = cellar::getImageBank().getImagePtr(linearizedName);
            else
            {
                std::shared_ptr<cellar::Image> img =
                    cellar::getImageBank().getImagePtr(name);

                _paintColorTexture.reset(new cellar::Image(*img));
                cellar::ImageSampler::linearize(*_paintColorTexture);

                cellar::getImageBank().addImage(linearizedName, *_paintColorTexture);
            }
        }
    }

    double TexturedStdCoating::paintRefractiveIndex(const glm::dvec3& tex) const
    {
        return _paintRefractiveIndex;
    }

    void TexturedStdCoating::setPaintRefractiveIndex(double refractiveIndex)
    {
        _paintRefractiveIndex = refractiveIndex;
    }

    void TexturedStdCoating::setTexFilter(const cellar::ESamplerFilter& filter)
    {
        _sampler.setFilter(filter);
    }

    void TexturedStdCoating::setTexWrapper(const cellar::ESamplerWrapper& wrapper)
    {
        _sampler.setWrapper(wrapper);
    }
}
