#include "QuadricEquation.h"
#include "Shape/AbstractShape.h"

namespace prop3
{
    // Plane Equation
    PlaneEquation::PlaneEquation(const glm::dvec3& normal, const glm::dvec3& point) :
        _normal(glm::normalize(normal)),
        _d(-glm::dot(_normal, point))
    {
    }

    PlaneEquation::PlaneEquation(double a, double b, double c, double d) :
        _normal(a, b, c),
        _d(d)
    {
    }

    EPointPosition PlaneEquation::isIn(const glm::dvec3& point) const
    {
        double dist = glm::dot(_normal, point) + _d;
        return dist < 0 ? EPointPosition::IN :
                    dist > 0.0 ? EPointPosition::OUT :
                        EPointPosition::ON;
    }

    double PlaneEquation::signedDistance(const glm::dvec3& point) const
    {
        return glm::dot(_normal, point) + _d;
    }

    void PlaneEquation::raycast(const Ray& ray, std::vector<RaycastReport>& reports) const
    {
        double dirDotNorm = glm::dot(_normal, ray.direction);
        if(dirDotNorm != 0.0)
        {
            double t = -(glm::dot(_normal, ray.origin) + _d) / dirDotNorm;
            glm::dvec3 pt = ray.origin + ray.direction * t;
            reports.push_back(RaycastReport(pt, _normal, t));
        }
    }



    // Sphere Equation
    SphereEquation::SphereEquation(const glm::dvec3& center, double radius) :
        _radius(radius),
        _radius2(radius*radius),
        _center(center)
    {

    }

    EPointPosition SphereEquation::isIn(const glm::dvec3& point) const
    {
        glm::dvec3 dist = point - _center;
        double dist2 = glm::dot(dist, dist);
        return dist2 < _radius2 ? EPointPosition::IN :
                    dist2 > _radius2 ? EPointPosition::OUT :
                        EPointPosition::ON;
    }

    double SphereEquation::signedDistance(const glm::dvec3& point) const
    {
        glm::dvec3 dist = point - _center;
        return glm::length(dist) -  _radius;
    }

    void SphereEquation::raycast(const Ray& ray, std::vector<RaycastReport>& reports) const
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
            reports.push_back(RaycastReport(pt, n, t1));

            double t2 = (-b + disrcSqrt) / (2 * a);
            pt = ray.origin + ray.direction*t2;
            n = glm::normalize(pt - _center);
            reports.push_back(RaycastReport(pt, n, t2));
        }
        else if(discr == 0.0)
        {
            double t = -b / (2 * a);
            glm::dvec3 pt = ray.origin + ray.direction*t;
            glm::dvec3 n = glm::normalize(pt - _center);
            reports.push_back(RaycastReport(pt, n, t));
        }
    }
}
