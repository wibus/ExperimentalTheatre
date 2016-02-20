#include "SphericalLight.h"

#include "Node/Visitor.h"
#include "Ray/Raycast.h"
#include "Ray/RayHitList.h"
#include "Ray/RayHitReport.h"
#include "Node/Light/LightCast.h"
#include "Node/Prop/Coating/EmissiveCoating.h"


namespace prop3
{
    SphericalLight::SphericalLight(
            const std::string& name,
            const glm::dvec3& center,
            double radius) :
        LightBulb(name),
        _center(center),
        _radius(radius)
    {

    }

    SphericalLight::~SphericalLight()
    {

    }

    void SphericalLight::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    void SphericalLight::raycast(const Raycast& ray, RayHitList& reports) const
    {
        glm::dvec3 orig = glm::dvec3(_invTransform * glm::dvec4(ray.origin, 1.0));
        glm::dvec3 dir = glm::dvec3(_invTransform * glm::dvec4(ray.direction, 0.0));

        glm::dvec3 dist = orig - _center;
        double a = glm::dot(dir, dir);
        double b = 2 * glm::dot(dir, dist);
        double c = glm::dot(dist, dist) - (_radius * _radius);

        double discr = b*b - 4*a*c;
        if(discr > 0.0)
        {
            double disrcSqrt = glm::sqrt(discr);

            double t1 = (-b - disrcSqrt) / (2 * a);
            if(0.0 < t1 && t1 < ray.limit)
            {
                glm::dvec3 pt = ray.origin + ray.direction*t1;
                glm::dvec3 n = glm::normalize(pt - _transformC);
                reports.add(t1, pt, n,
                            RayHitReport::NO_TEXCOORD,
                            _coating.get(),
                            nullptr,
                            _outerMat.get());
            }

            double t2 = (-b + disrcSqrt) / (2 * a);
            if(0.0 < t2 && t2 < ray.limit)
            {
                glm::dvec3 pt = ray.origin + ray.direction*t2;
                glm::dvec3 n = glm::normalize(pt - _transformC);
                reports.add(t2, pt, n,
                            RayHitReport::NO_TEXCOORD,
                            _coating.get(),
                            nullptr,
                            _outerMat.get());
            }
        }
        else if(discr == 0.0)
        {
            double t = -b / (2 * a);
            if(0.0 < t && t < ray.limit)
            {
                glm::dvec3 pt = ray.origin + ray.direction*t;
                glm::dvec3 n = glm::normalize(pt - _transformC);
                reports.add(t, pt, n,
                            RayHitReport::NO_TEXCOORD,
                            _coating.get(),
                            nullptr,
                            _outerMat.get());
            }
        }
    }

    bool SphericalLight::intersects(const Raycast& ray, RayHitList& reports) const
    {
        glm::dvec3 orig = glm::dvec3(_invTransform * glm::dvec4(ray.origin, 1.0));
        glm::dvec3 dir = glm::dvec3(_invTransform * glm::dvec4(ray.direction, 0.0));


        glm::dvec3 dist = orig - _center;
        double a = glm::dot(dir, dir);
        double b = 2 * glm::dot(dir, dist);
        double c = glm::dot(dist, dist) - (_radius * _radius);

        double discr = b*b - 4*a*c;
        if(discr > 0.0)
        {
            double disrcSqrt = glm::sqrt(discr);

            double t1 = (-b - disrcSqrt) / (2 * a);
            if(0.0 < t1 && t1 < ray.limit)
            {
                return true;
            }

            double t2 = (-b + disrcSqrt) / (2 * a);
            if(0.0 < t2 && t2 < ray.limit)
            {
                return true;
            }
        }
        else if(discr == 0.0)
        {
            double t = -b / (2 * a);
            if(0.0 < t && t < ray.limit)
            {
                return true;
            }
        }

        return false;
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
