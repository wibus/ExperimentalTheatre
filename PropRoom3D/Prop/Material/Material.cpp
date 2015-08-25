#include "Material.h"

#include "../Ray/Raycast.h"


namespace prop3
{
    const double Material::INFINITE_DENSITY = 0.0;

    Material::Material() :
        _refractiveIndex(1.0),
        _density(1.0),
        _bounciness(1.0),
        _staticFrictionCoefficient(1.0),
        _dynamicFrictionCoefficient(1.0)
    {

    }

    Material::~Material()
    {

    }

    void Material::setRefractiveIndex(double refractiveIndex)
    {
        _refractiveIndex = refractiveIndex;
    }


    double Material::lightFreePathLength(const Ray& ray) const
    {
        return Ray::BACKDROP_DISTANCE;
    }

    glm::dvec3 Material::lightAttenuation(
                const Ray& ray,
                double distance) const
    {
        return glm::dvec3(1.0);
    }

    void Material::scatterLight(
            std::vector<Raycast>& raycasts,
            const Ray& ray, double distance,
            const std::shared_ptr<Material>& self,
            unsigned int outRayCountHint) const
    {
        Ray scatter = ray;
        scatter.origin += ray.direction * distance;
        raycasts.push_back(Raycast(scatter, glm::dvec3(1.0), self));
    }

    void Material::setDensity(const double& density)
    {
        _density = density;
    }

    void Material::setBounciness(const double& bounciness)
    {
        _bounciness = bounciness;
    }

    void Material::setStaticFrictionCoefficient(const double& us)
    {
        _staticFrictionCoefficient = us;
    }

    void Material::setDynamicFrictionCoefficient(const double& ud)
    {
        _dynamicFrictionCoefficient = ud;
    }
}
