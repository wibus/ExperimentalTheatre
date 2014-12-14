#include "Sphere.h"
#include "Raycast.h"

namespace prop3
{
    Sphere::Sphere(const glm::dvec3& center, double radius) :
        _radius(radius),
        _radius2(radius*radius),
        _center(center)
    {

    }

    Sphere::~Sphere()
    {

    }

    void Sphere::transform(const Transform& transform)
    {
        _center = glm::dvec3(transform.mat() * glm::dvec4(_center, 1.0));

        glm::dvec4 unit = glm::dvec4(1.0, 0.0, 0.0, 0.0);
        _radius *= glm::length(transform.mat() * unit);
        _radius2 = _radius*_radius;
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

    void Sphere::raycast(const Ray& ray, std::vector<RaycastReport>& reports) const
    {
        glm::dvec3 dist = ray.origin - _center;

        double a = glm::dot(ray.direction, ray.direction);
        double b = 2 * glm::dot(ray.direction, dist);
        double c = glm::dot(dist, dist) - _radius2;

        double discr = b*b - 4*a*c;
        if(discr > 0.0)
        {
            double disrcSqrt = glm::sqrt(discr);

            double t1 = (-b - disrcSqrt) / (2 * a);
            glm::dvec3 pt = ray.origin + ray.direction*t1;
            glm::dvec3 n = glm::normalize(pt - _center);
            reports.push_back(RaycastReport(t1, pt, n));

            double t2 = (-b + disrcSqrt) / (2 * a);
            pt = ray.origin + ray.direction*t2;
            n = glm::normalize(pt - _center);
            reports.push_back(RaycastReport(t2, pt, n));
        }
        else if(discr == 0.0)
        {
            double t = -b / (2 * a);
            glm::dvec3 pt = ray.origin + ray.direction*t;
            glm::dvec3 n = glm::normalize(pt - _center);
            reports.push_back(RaycastReport(t, pt, n));
        }
    }
}
