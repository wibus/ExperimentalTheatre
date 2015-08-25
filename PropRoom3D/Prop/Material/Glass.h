#ifndef PROPROOM3D_GLASS_H
#define PROPROOM3D_GLASS_H

#include "Material.h"


namespace prop3
{

    class PROP3D_EXPORT Glass :
            public Material
    {
    public:
        Glass(const glm::dvec3& color,
              double concentration,
              double refractiveIndex = 1.5);
        virtual ~Glass();

        // SceneNode interface
        virtual void accept(SceneVisitor& visitor) override;


        virtual void brdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& selfEnteredMaterial,
            unsigned int outRayCountHint) const override;

        virtual glm::dvec3 lightAttenuation(
            const Ray& ray,
            double distance) const override;

        glm::dvec3 color() const;

        double concentration() const;


    protected:
        glm::dvec3 _color;
        double _concentration;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 Glass::color() const
    {
        return _color;
    }

    inline double Glass::concentration() const
    {
        return _concentration;
    }
}

#endif // PROPROOM3D_GLASS_H
