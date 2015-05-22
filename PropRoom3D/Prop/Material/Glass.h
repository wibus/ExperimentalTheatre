#ifndef PROPROOM3D_GLASS_H
#define PROPROOM3D_GLASS_H

#include "Material.h"


namespace prop3
{

    class PROP3D_EXPORT Glass :
            public Material
    {
    public:
        Glass(const glm::dvec3& color, double dyeConcentration);
        virtual ~Glass();

        virtual void brdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& selfEnteredMaterial,
            unsigned int outRayCountHint) const override;

        virtual glm::dvec3 lightAttenuation(
            const Ray& ray,
            double distance) const override;

    protected:
        glm::dvec3 _color;
        double _dyeConcentration;
    };
}

#endif // PROPROOM3D_GLASS_H
