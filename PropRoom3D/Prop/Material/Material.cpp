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


    double Material::lightFreePathLength(const Raycast& ray) const
    {
        return ray.limit;
    }

    glm::dvec3 Material::lightAttenuation(const Raycast& ray) const
    {
        return glm::dvec3(1.0);
    }

    void Material::scatterLight(
            std::vector<Raycast>& raycasts,
            const Raycast& ray, double distance,
            const std::shared_ptr<Material>& self,
            unsigned int outRayCountHint) const
    {
        glm::dvec3 origin = ray.direction * distance;
        raycasts.push_back(Raycast(
            Raycast::BACKDROP_DISTANCE,
            Raycast::FULLY_SPECULAR_ENTROPY,
            glm::dvec3(1.0),
            origin,
            ray.direction,
            self));
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
