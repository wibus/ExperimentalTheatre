#ifndef PROPROOM3D_MATERIAL_H
#define PROPROOM3D_MATERIAL_H

#include <memory>
#include <vector>

#include <GLM/glm.hpp>

#include "PropRoom3D/StageSet/StageSetNode.h"


namespace prop3
{
    class Raycast;
    class Raycast;
    class RayHitReport;


    class PROP3D_EXPORT Material : public StageSetNode
    {
    protected:
        Material();

    public:
        virtual ~Material();


        // OPTICAL PROPERTIES
        double refractiveIndex() const;
        void setRefractiveIndex(double refractiveIndex);


        virtual void indirectBrdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& selfEnteredMaterial,
            unsigned int outRayCountHint) const = 0;

        virtual glm::dvec3 directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial) const = 0;

        virtual double lightFreePathLength(
            const Raycast& ray) const;

        virtual glm::dvec3 lightAttenuation(
            const Raycast& ray) const;

        virtual void scatterLight(
            std::vector<Raycast>& raycasts,
            const Raycast& ray, double distance,
            const std::shared_ptr<Material>& self,
            unsigned int outRayCountHint) const;


        // MECANICAL PROPERTIES
        double density() const;
        double bounciness() const;
        double staticFrictionCoefficient() const;
        double dynamicFrictionCoefficient() const;

        void setDensity(const double& density);
        void setBounciness(const double& bounciness);
        void setStaticFrictionCoefficient(const double& us);
        void setDynamicFrictionCoefficient(const double& ud);


        // Constant attributes
        static const double INFINITE_DENSITY;


    protected:
        // OPTICAL PROPERTIES
        double _refractiveIndex;


        // MECANICAL PROPERTIES
        double _density;
        double _staticFrictionCoefficient;
        double _dynamicFrictionCoefficient;
        double _bounciness;
    };



    // IMPLEMENTATION //
    inline double Material::refractiveIndex() const
    {
        return _refractiveIndex;
    }

    inline double Material::density() const
    {
        return _density;
    }

    inline double Material::bounciness() const
    {
        return _bounciness;
    }

    inline double Material::staticFrictionCoefficient() const
    {
        return _staticFrictionCoefficient;
    }

    inline double Material::dynamicFrictionCoefficient() const
    {
        return _dynamicFrictionCoefficient;
    }
}

#endif // PROPROOM3D_MATERIAL_H
