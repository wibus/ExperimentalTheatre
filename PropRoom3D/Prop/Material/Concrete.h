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

        // SceneNode interface
        virtual void accept(SceneVisitor& visitor) override;


        virtual void brdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& selfEnteredMaterial,
            unsigned int outRayCountHint) const override;

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
