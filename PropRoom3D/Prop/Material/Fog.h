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

        // SceneNode interface
        virtual void accept(SceneVisitor& visitor) override;


        virtual void brdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& selfEnteredMaterial,
            unsigned int outRayCountHint) const override;

        virtual double lightFreePathLength(
            const Ray& ray) const;

        virtual void scatterLight(
            std::vector<Raycast>& raycasts,
            const Ray& ray, double distance,
            const std::shared_ptr<Material>& self,
            unsigned int outRayCountHint) const;

        glm::dvec3 color() const;

        double concentration() const;

        double radius() const;

    protected:
        double getDistanceInFog(const Ray& ray, double rayDistance) const;
        double getDistanceInFog(const Ray& ray, double rayDistance, double& enterDist) const;


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
