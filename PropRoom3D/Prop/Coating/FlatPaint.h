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
            const Material& leavedMaterial,
            const Material& enteredMaterial,
            unsigned int outRayCountHint) const override;

        virtual glm::dvec3 directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const override;


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
