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


        virtual void brdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& selfEnteredMaterial,
            unsigned int outRayCountHint)  const override;

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
