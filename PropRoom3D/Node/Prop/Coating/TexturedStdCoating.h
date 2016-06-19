#ifndef PROPROOM3D_TEXTUREDSTDCOATING_H
#define PROPROOM3D_TEXTUREDSTDCOATING_H

#include <CellarWorkbench/Image/TextureSampler.h>

#include "StdCoating.h"


namespace prop3
{
    class PROP3D_EXPORT TexturedStdCoating : public StdCoating
    {
    public:
        TexturedStdCoating();
        virtual ~TexturedStdCoating();

        // Node interface
        virtual void accept(Visitor& visitor) override;


        virtual double roughness(const glm::dvec3& tex) const override;
        void setDefaultRoughness(double roughness);
        double defaultRoughness() const;
        void setRoughnessTexName(const std::string& name);
        std::string roughnessTexName() const;


        virtual glm::dvec4 paintColor(const glm::dvec3& tex) const override;
        void setDefaultPaintColor(const glm::dvec4& color);
        glm::dvec4 defaultPaintColor() const;
        void setPaintColorTexName(const std::string& name);
        std::string paintColorTexName() const;


        virtual double paintRefractiveIndex(const glm::dvec3& tex) const override;
        void setPaintRefractiveIndex(double refractiveIndex);
        double paintRefractiveIndex() const;


        void setTexFilter(const cellar::ESamplerFilter& filter);
        cellar::ESamplerFilter texFilter() const;

        void setTexWrapper(const cellar::ESamplerWrapper& wrapper);
        cellar::ESamplerWrapper texWrapper() const;


        static const std::string NO_TEXTURE;

    private:
        double _defaultRoughness;
        std::string _roughnessTexName;
        glm::dvec4 _defaultPaintColor;
        std::string _paintColorTexName;
        double _paintRefractiveIndex;

        cellar::TextureSampler _sampler;
        std::shared_ptr<cellar::Texture> _roughnessTexture;
        std::shared_ptr<cellar::Texture> _paintColorTexture;
    };



    // IMPLEMENTATION //
    inline double TexturedStdCoating::defaultRoughness() const
    {
        return _defaultRoughness;
    }

    inline std::string TexturedStdCoating::roughnessTexName() const
    {
        return _roughnessTexName;
    }

    inline glm::dvec4 TexturedStdCoating::defaultPaintColor() const
    {
        return _defaultPaintColor;
    }

    inline std::string TexturedStdCoating::paintColorTexName() const
    {
        return _paintColorTexName;
    }

    inline double TexturedStdCoating::paintRefractiveIndex() const
    {
        return _paintRefractiveIndex;
    }

    inline cellar::ESamplerFilter TexturedStdCoating::texFilter() const
    {
        return _sampler.filter();
    }

    inline cellar::ESamplerWrapper TexturedStdCoating::texWrapper() const
    {
        return _sampler.wrapper();
    }
}

#endif // PROPROOM3D_TEXTUREDSTDCOATING_H
