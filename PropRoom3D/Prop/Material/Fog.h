#ifndef PROPROOM3D_FOG_H
#define PROPROOM3D_FOG_H

#include <random>

#include "Air.h"


namespace prop3
{
    class Surface;


    class PROP3D_EXPORT Fog :
            public Air
    {
    public:
        Fog(const glm::dvec3& color,
            double concentration,
            double radius,
            double refractiveIndex = AIR_REFRACTIVE_INDEX);
        virtual ~Fog();


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
            const Material& enteredMaterial) const override;

        virtual double lightFreePathLength(
            const Raycast& ray) const override;

        virtual void scatterLight(
            std::vector<Raycast>& raycasts,
            const Raycast& ray,
            unsigned int outRayCountHint) const override;

        virtual glm::dvec3 gatherLight(
            const Raycast& ray,
            const glm::dvec3& outDirection) const override;

        glm::dvec3 color() const;

        double concentration() const;

        double radius() const;

    protected:
        double getDistanceInFog(const Raycast& ray, double& enterDist) const;


        glm::dvec3 _color;
        double _concentration;
        double _radius;


        std::shared_ptr<Surface> _fogGeometry;
        mutable std::mt19937 _randomEngine;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 Fog::color() const
    {
        return _color;
    }

    inline double Fog::concentration() const
    {
        return _concentration;
    }

    inline double Fog::radius() const
    {
        return _radius;
    }
}

#endif // PROPROOM3D_FOG_H
