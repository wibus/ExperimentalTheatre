#include "Plane.h"
#include "Prop/Prop.h"

namespace prop3
{
    Plane::Plane(const glm::dvec3& normal, const glm::dvec3& point) :
        _normal(glm::normalize(normal)),
        _d(-glm::dot(_normal, point))
    {
    }

    Plane::Plane(double a, double b, double c, double d) :
        _normal(a, b, c),
        _d(d)
    {
    }

    Plane::~Plane()
    {

    }

    void Plane::transform(const Transform& transform)
    {
        glm::dvec4 pt((-_d) * _normal, 1.0);
        _normal = glm::dvec3(transform.mat() * glm::dvec4(_normal, 0.0));
    }

    EPointPosition Plane::isIn(const glm::dvec3& point) const
    {
        double dist = glm::dot(_normal, point) + _d;
        return dist < 0 ? EPointPosition::IN :
                    dist > 0.0 ? EPointPosition::OUT :
                        EPointPosition::ON;
    }

    double Plane::signedDistance(const glm::dvec3& point) const
    {
        return glm::dot(_normal, point) + _d;
    }

    void Plane::raycast(const Ray& ray, std::vector<RaycastReport>& reports) const
    {
        double dirDotNorm = glm::dot(_normal, ray.direction);
        if(dirDotNorm != 0.0)
        {
            double t = -(glm::dot(_normal, ray.origin) + _d) / dirDotNorm;
            glm::dvec3 pt = ray.origin + ray.direction * t;
            reports.push_back(RaycastReport(pt, _normal, t));
        }
    }
}
