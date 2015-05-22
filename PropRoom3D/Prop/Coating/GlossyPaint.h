#ifndef PROPROOM3D_GLOSSYPAINT_H
#define PROPROOM3D_GLOSSYPAINT_H

#include "Coating.h"


namespace prop3
{

    class PROP3D_EXPORT GlossyPaint :
            public Coating
    {
    public:
        GlossyPaint(const glm::dvec3& color,
                    double glossiness = 0.0,
                    double varnishRefractiveIndex = 1.55);
        virtual ~GlossyPaint();

        virtual void brdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial,
            unsigned int outRayCountHint) const override;

    protected:
        glm::dvec3 _color;
        double _glossiness;
        double _varnishRefractiveIndex;
    };
}

#endif // PROPROOM3D_GLOSSYPAINT_H
