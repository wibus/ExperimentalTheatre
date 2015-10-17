#ifndef PROPROOM3D_TEXTUREDFLATPAINT_H
#define PROPROOM3D_TEXTUREDFLATPAINT_H

#include <CellarWorkbench/Image/ImageSampler.h>

#include "Coating.h"


namespace prop3
{

    class PROP3D_EXPORT TexturedFlatPaint :
            public Coating
    {
    public:
        TexturedFlatPaint(
            const std::string& texName,
            const cellar::ESamplerFilter& texFilter = cellar::ESamplerFilter::NEAREST,
            const cellar::ESamplerWrapper& texWrapper = cellar::ESamplerWrapper::CLAMP,
            const glm::dvec3& defaultColor = glm::dvec3(1.0));
        virtual ~TexturedFlatPaint();

        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;


        virtual void indirectBrdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const Material& leavedMaterial,
            const Material& enteredMaterial,
            unsigned int outRayCountHint) const override;

        virtual glm::dvec3 directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const override;


        std::string texName() const;

        cellar::ESamplerFilter texFilter() const;

        cellar::ESamplerWrapper texWrapper() const;

        glm::dvec3 defaultColor() const;

    private:
        std::string _texName;
        cellar::Image& _texture;
        cellar::ImageSampler _sampler;
        glm::dvec3 _defaultColor;
    };



    // IMPLEMENTATION //
    inline std::string TexturedFlatPaint::texName() const
    {
        return _texName;
    }

    inline cellar::ESamplerFilter TexturedFlatPaint::texFilter() const
    {
        return _sampler.filter();
    }

    inline cellar::ESamplerWrapper TexturedFlatPaint::texWrapper() const
    {
        return _sampler.wrapper();
    }

    inline glm::dvec3 TexturedFlatPaint::defaultColor() const
    {
        return _defaultColor;
    }
}

#endif // PROPROOM3D_TEXTUREDFLATPAINT_H
