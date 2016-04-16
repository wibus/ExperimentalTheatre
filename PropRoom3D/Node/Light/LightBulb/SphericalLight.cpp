#include "SphericalLight.h"

#include "Node/Visitor.h"
#include "Ray/Raycast.h"
#include "Ray/RayHitList.h"
#include "Ray/RayHitReport.h"
#include "Node/Light/LightCast.h"
#include "Node/Prop/Surface/Sphere.h"
#include "Node/Prop/Coating/EmissiveCoating.h"


namespace prop3
{
    SphericalLight::SphericalLight(
            const std::string& name,
            const glm::dvec3& center,
            double radius) :
        LightBulb(name),
        _center(center),
        _radius(radius),
        _radius2(radius*radius),
        _transformC(center)
    {
        setSurface(Sphere::sphere(center, radius));
    }

    SphericalLight::~SphericalLight()
    {

    }

    void SphericalLight::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    void SphericalLight::fireOn(
            std::vector<LightCast>& lightCasts,
            const glm::dvec3& pos,
            unsigned int count) const
    {
        if(!_isOn) return;

        glm::dvec4 sample(radiantFlux() / area(), 1.0);

        for(int i=0; i < count; ++i)
        {
            glm::dvec3 randRad = _sphereRand.gen(_radius);
            glm::dvec3 source = _transformC + randRad;

            glm::dvec3 dir = pos - source;
            double dotDir = glm::dot(dir, randRad);
            if(dotDir < 0.0)
            {
                source = _transformC - randRad;
                dir = pos - source;
            }

            dir = glm::normalize(dir);

            Raycast ray = Raycast(0.0, sample, source, dir);

            using namespace std::placeholders;
            lightCasts.push_back(LightCast(ray, source, dir,
                std::bind(&SphericalLight::diffuseSize, this, _1, _2, _3)));
        }
    }

    double SphericalLight::area() const
    {
        return 4.0 * glm::pi<double>() * _radius * _radius;
    }

    double SphericalLight::visibility(const Raycast& ray) const
    {
        glm::dvec3 dist = ray.origin - _transformC;

        double length = glm::max(glm::length(dist), _radius);
        double span = 1.0 - glm::cos(glm::asin(_radius / length));

        return span;
    }

    void SphericalLight::onTransform()
    {
        _transformC = glm::dvec3(_transform * glm::dvec4(_center, 1.0));
    }
}
