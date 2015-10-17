#ifndef PROPROOM3D_CONCRETE_H
#define PROPROOM3D_CONCRETE_H

#include "Material.h"


namespace prop3
{

    class PROP3D_EXPORT Concrete :
            public Material
    {
    public:
        Concrete(const glm::dvec3& color);
        virtual ~Concrete();

        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;


        virtual void indirectBrdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const Material& leavedMaterial,
            const Material& enteredMaterial,
            unsigned int outRayCountHint) const override;

        virtual glm::dvec3 directBrdf(
            const RayHitReport &report,
            const glm::dvec3& outDirection,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const;

        virtual glm::dvec3 lightAttenuation(
            const Raycast& ray) const override;

        glm::dvec3 color() const;


    protected:
        glm::dvec3 _color;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 Concrete::color() const
    {
        return _color;
    }
}

#endif // PROPROOM3D_CONCRETE_H
