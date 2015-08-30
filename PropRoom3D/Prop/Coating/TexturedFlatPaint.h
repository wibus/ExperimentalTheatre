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

        glm::dvec3 defaultColor() const;

    private:
        std::string _texName;
        cellar::Image& _texture;
        glm::dvec3 _defaultColor;
    };



    // IMPLEMENTATION //
    inline std::string TexturedFlatPaint::texName() const
    {
        return _texName;
    }

    inline glm::dvec3 TexturedFlatPaint::defaultColor() const
    {
        return _defaultColor;
    }
}

#endif // PROPROOM3D_TEXTUREDFLATPAINT_H
