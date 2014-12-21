#include "Plane.h"
#include "Raycast.h"

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
            reports.push_back(RaycastReport(t, pt, _normal));
        }
    }


    // Ghost
    PlaneGhost::PlaneGhost(const glm::dvec3& normal, const glm::dvec3& point) :
        Plane(normal, point)
    {
    }

    PlaneGhost::PlaneGhost(double a, double b, double c, double d) :
        Plane(a, b, c, d)
    {
    }

    void PlaneGhost::raycast(const Ray&, std::vector<RaycastReport>&) const
    {
    }


    // Textures
    // Where normal is (a, b, c) and point is on the plane
    PlaneTexture::PlaneTexture(const glm::dvec3& normal, const glm::dvec3& point,
                               const glm::dvec3& u, const glm::dvec3& v,
                               const glm::dvec3& origin) :
        Plane(normal, point),
        _origin(origin),
        _u(u), _v(v)
    {

    }

    // From the equation : a*x + b*y + c*z + d = 0
    PlaneTexture::PlaneTexture(double a, double b, double c, double d,
                               const glm::dvec3& u, const glm::dvec3& v,
                               const glm::dvec3& origin) :
        Plane(a, b, c, d),
        _origin(origin),
        _u(u), _v(v)
    {

    }

    void PlaneTexture::raycast(const Ray& ray, std::vector<RaycastReport>& reports) const
    {
        std::vector<RaycastReport> planeReports;
        Plane::raycast(ray, planeReports);

        for(RaycastReport& r : planeReports)
        {
            glm::dvec3 dist = r.position - _origin;
            r.texCoord.s = glm::dot(dist, _u) / glm::dot(_u, _u);
            r.texCoord.t = glm::dot(dist, _v) / glm::dot(_v, _v);
        }

        reports.insert(reports.end(), planeReports.begin(), planeReports.end());
    }
}
