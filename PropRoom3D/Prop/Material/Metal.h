#ifndef PROPROOM3D_CHROME_H
#define PROPROOM3D_CHROME_H

#include "Material.h"


namespace prop3
{

    class PROP3D_EXPORT Metal :
            public Material
    {
    public:
        Metal(const glm::dvec3& color, double glossiness = 1.0);
        virtual ~Metal();

        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;


        virtual void indirectBrdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& selfEnteredMaterial,
            unsigned int outRayCountHint)  const override;

        virtual glm::dvec3 directBrdf(
            const RayHitReport &report,
            const glm::dvec3& outDirection,
            const std::shared_ptr<Material> &leavedMaterial,
            const std::shared_ptr<Material> &enteredMaterial) const;

        virtual glm::dvec3 lightAttenuation(
            const Raycast& ray) const override;

        glm::dvec3 color() const;

        double glossiness() const;


    protected:
        glm::dvec3 _color;
        double _glossiness;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 Metal::color() const
    {
        return _color;
    }

    inline double Metal::glossiness() const
    {
        return _glossiness;
    }
}

#endif // PROPROOM3D_CHROME_H
