#include "Sphere.h"

#include "../Coating/Coating.h"
#include "Ray/RayHitList.h"
#include "Ray/RayHitReport.h"
#include "../../StageSet/StageSetVisitor.h"


namespace prop3
{
    Sphere::Sphere(const glm::dvec3& center, double radius) :
        _radius(radius),
        _radius2(radius*radius),
        _center(center)
    {

    }

    std::shared_ptr<Surface>
        Sphere::sphere(const glm::dvec3& center, double radius)
    {
        return std::shared_ptr<Surface>(new Sphere(center, radius));
    }

    void Sphere::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }

    EPointPosition Sphere::isIn(const glm::dvec3& point) const
    {
        glm::dvec3 dist = point - _center;
        double dist2 = glm::dot(dist, dist);
        return dist2 < _radius2 ? EPointPosition::IN :
                    dist2 > _radius2 ? EPointPosition::OUT :
                        EPointPosition::ON;
    }

    double Sphere::signedDistance(const glm::dvec3& point) const
    {
        glm::dvec3 dist = point - _center;
        return glm::length(dist) -  _radius;
    }

    void Sphere::raycast(const Raycast& ray, RayHitList& reports) const
    {
        double a, b, c;
        params(ray, a, b, c);
        double discr = b*b - 4*a*c;
        if(discr > 0.0)
        {
            double disrcSqrt = glm::sqrt(discr);

            double t1 = (-b - disrcSqrt) / (2 * a);
            if(0.0 < t1 && t1 < ray.limit)
            {
                glm::dvec3 pt = ray.origin + ray.direction*t1;
                glm::dvec3 n = glm::normalize(pt - _center);
                reports.add(t1, ray, pt, n,
                            RayHitReport::NO_TEXCOORD,
                            _coating.get(),
                            _innerMat.get(),
                            _outerMat.get());
            }

            double t2 = (-b + disrcSqrt) / (2 * a);
            if(0.0 < t2 && t2 < ray.limit)
            {
                glm::dvec3 pt = ray.origin + ray.direction*t2;
                glm::dvec3 n = glm::normalize(pt - _center);
                reports.add(t2, ray, pt, n,
                            RayHitReport::NO_TEXCOORD,
                            _coating.get(),
                            _innerMat.get(),
                            _outerMat.get());
            }
        }
        else if(discr == 0.0)
        {
            double t = -b / (2 * a);
            if(0.0 < t && t < ray.limit)
            {
                glm::dvec3 pt = ray.origin + ray.direction*t;
                glm::dvec3 n = glm::normalize(pt - _center);
                reports.add(t, ray, pt, n,
                            RayHitReport::NO_TEXCOORD,
                            _coating.get(),
                            _innerMat.get(),
                            _outerMat.get());
            }
        }
    }

    bool Sphere::intersects(const Raycast& ray, RayHitList& reports) const
    {
        double a, b, c;
        params(ray, a, b, c);
        return b*b - 4*a*c >= 0.0;
    }

    void Sphere::transform(const Transform& transform)
    {
        _center = glm::dvec3(transform.mat() * glm::dvec4(_center, 1.0));

        glm::dvec4 unit = glm::dvec4(1.0, 0.0, 0.0, 0.0);
        _radius *= glm::length(transform.mat() * unit);
        _radius2 = _radius*_radius;

        stampCurrentUpdate();
    }

    void Sphere::params(const Raycast& ray, double& a, double& b, double& c) const
    {
        glm::dvec3 dist = ray.origin - _center;

        a = glm::dot(ray.direction, ray.direction);
        b = 2 * glm::dot(ray.direction, dist);
        c = glm::dot(dist, dist) - _radius2;
    }
}
