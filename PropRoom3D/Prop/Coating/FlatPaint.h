#ifndef PROPROOM3D_FLATPAINT_H
#define PROPROOM3D_FLATPAINT_H

#include "Coating.h"


namespace prop3
{

    class PROP3D_EXPORT FlatPaint :
            public Coating
    {
    public:
        FlatPaint(const glm::dvec3& color);
        virtual ~FlatPaint();

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


        glm::dvec3 color() const;


    protected:
        glm::dvec3 _color;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 FlatPaint::color() const
    {
        return _color;
    }
}

#endif // PROPROOM3D_FLATPAINT_H
