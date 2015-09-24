#ifndef PROPROOM3D_TEXTUREDGLOSSYPAINT_H
#define PROPROOM3D_TEXTUREDGLOSSYPAINT_H

#include <CellarWorkbench/Image/ImageSampler.h>

#include "Coating.h"


namespace prop3
{

    class PROP3D_EXPORT TexturedGlossyPaint :
            public Coating
    {
    public:
        TexturedGlossyPaint(
            const std::string& texName,
            const std::string& glossName,
            const cellar::ESamplerFilter& texFilter = cellar::ESamplerFilter::NEAREST,
            const cellar::ESamplerWrapper& texWrapper = cellar::ESamplerWrapper::CLAMP,
            const glm::dvec3& defaultColor = glm::dvec3(1.0),
            double defaultGlossiness = 0.0,
            double varnishRefractiveIndex = 1.55);
        virtual ~TexturedGlossyPaint();

        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;


        virtual void indirectBrdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial,
            unsigned int outRayCountHint) const override;

        virtual glm::dvec3 directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial) const override;


        std::string texName() const;

        std::string glossName() const;

        glm::dvec3 defaultColor() const;

        cellar::ESamplerFilter texFilter() const;

        cellar::ESamplerWrapper texWrapper() const;

        double defaultGlossiness() const;

        double varnishRefractiveIndex() const;


    private:
        std::string _texName;
        std::string _glossName;
        cellar::Image& _texture;
        cellar::Image& _glossMap;
        cellar::ImageSampler _sampler;
        glm::dvec3 _defaultColor;
        double _defaultGlossiness;
        double _varnishRefractiveIndex;
    };



    // IMPLEMENTATION //
    inline std::string TexturedGlossyPaint::texName() const
    {
        return _texName;
    }

    inline std::string TexturedGlossyPaint::glossName() const
    {
        return _glossName;
    }

    inline cellar::ESamplerFilter TexturedGlossyPaint::texFilter() const
    {
        return _sampler.filter();
    }

    inline cellar::ESamplerWrapper TexturedGlossyPaint::texWrapper() const
    {
        return _sampler.wrapper();
    }

    inline glm::dvec3 TexturedGlossyPaint::defaultColor() const
    {
        return _defaultColor;
    }

    inline double TexturedGlossyPaint::defaultGlossiness() const
    {
        return _defaultGlossiness;
    }

    inline double TexturedGlossyPaint::varnishRefractiveIndex() const
    {
        return _varnishRefractiveIndex;
    }
}

#endif // PROPROOM3D_TEXTUREDGLOSSYPAINT_H
