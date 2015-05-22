#ifndef PROPROOM3D_TEXTUREDFLATPAINT_H
#define PROPROOM3D_TEXTUREDFLATPAINT_H

#include "Coating.h"


namespace cellar
{
    class Image;
}


namespace prop3
{

    class PROP3D_EXPORT TexturedFlatPaint :
            public Coating
    {
    public:
        TexturedFlatPaint(const std::string& texName,
                      const glm::dvec3& defaultColor = glm::dvec3(1.0));
        virtual ~TexturedFlatPaint();

        virtual void brdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial,
            unsigned int outRayCountHint) const override;

    private:
        cellar::Image& _texture;
        glm::dvec3 _defaultColor;
    };
}

#endif // PROPROOM3D_TEXTUREDFLATPAINT_H
