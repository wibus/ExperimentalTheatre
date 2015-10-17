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
