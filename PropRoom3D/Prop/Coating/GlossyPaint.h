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

        double glossiness() const;

        double varnishRefractiveIndex() const;


    protected:
        glm::dvec3 _color;
        double _glossiness;
        double _varnishRefractiveIndex;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 GlossyPaint::color() const
    {
        return _color;
    }

    inline double GlossyPaint::glossiness() const
    {
        return _glossiness;
    }

    inline double GlossyPaint::varnishRefractiveIndex() const
    {
        return _varnishRefractiveIndex;
    }
}

#endif // PROPROOM3D_GLOSSYPAINT_H
