#include "Quadric.h"

#include "../Ray/RayHitReport.h"


namespace prop3
{
    inline glm::dvec3 computeNormal(const glm::dmat4& Q, const glm::dvec3& pt)
    {
        double nx = Q[0][3] + Q[3][0] +
                    2 * Q[0][0] * pt.x +
                   (Q[0][1] + Q[1][0]) * pt.y +
                   (Q[0][2] + Q[2][0]) * pt.z;

        double ny = Q[1][3] + Q[3][1] +
                    2 * Q[1][1] * pt.y +
                   (Q[0][1] + Q[1][0]) * pt.x +
                   (Q[1][2] + Q[2][1]) * pt.z;

        double nz = Q[2][3] + Q[3][2] +
                    2 * Q[2][2] * pt.z +
                   (Q[0][2] + Q[2][0]) * pt.x +
                   (Q[1][2] + Q[2][1]) * pt.y;

        return glm::dvec3(nx, ny, nz);
    }

    Quadric::Quadric(const glm::dmat4& Q) :
        _q(Q),
        _coating(ImplicitSurface::NO_COATING)
    {

    }

    Quadric::Quadric(double A, double B, double C,
                     double D, double E, double F,
                     double G, double H, double I,
                     double J) :
        _q(A, B, C, D,
                  B, E, F, G,
                  C, F, H, I,
                  D, G, I, J),
        _coating(ImplicitSurface::NO_COATING)
    {

    }

    Quadric::~Quadric()
    {

    }


    void Quadric::transform(const Transform& transform)
    {
        _q = transform.inv() * _q * glm::transpose(transform.inv());
    }

    EPointPosition Quadric::isIn(const glm::dvec3& point) const
    {
        glm::dvec4 homoPt = glm::dvec4(point, 1);
        double dist = glm::dot(homoPt, _q * homoPt);
        return dist < 0.0 ? EPointPosition::IN :
                    dist > 0.0 ? EPointPosition::OUT :
                                    EPointPosition::ON;
    }

    double Quadric::signedDistance(const glm::dvec3& point) const
    {
        glm::dvec4 homoPt = glm::dvec4(point, 1);
        return glm::dot(homoPt, _q * homoPt);
    }

    // ref : http://marctenbosch.com/photon/mbosch_intersection.pdf
    void Quadric::raycast(const Ray& ray, std::vector<RayHitReport>& reports) const
    {
        double a, b, c;
        params(ray, a, b, c);

        if(a != 0.0)
        {
            double dscr = b*b - 4*a*c;
            if(dscr > 0.0)
            {
                double dsrcSqrt = glm::sqrt(dscr);

                {
                    double t1 = (-b - dsrcSqrt) / (2 * a);
                    glm::dvec3 pt1 = ray.origin + ray.direction*t1;
                    glm::dvec3 n1 =  computeNormal(_q, pt1);
                    reports.push_back(RayHitReport(ray, t1, pt1, n1, _coating));
                }

                {
                    double t2 = (-b + dsrcSqrt) / (2 * a);
                    glm::dvec3 pt2 = ray.origin + ray.direction*t2;
                    glm::dvec3 n2 =  computeNormal(_q, pt2);
                    reports.push_back(RayHitReport(ray, t2, pt2, n2, _coating));
                }
            }
            else if (dscr == 0.0)
            {
                double t = -b / (2 * a);
                glm::dvec3 pt = ray.origin + ray.direction*t;
                glm::dvec3 n =  computeNormal(_q, pt);
                reports.push_back(RayHitReport(ray, t, pt, n, _coating));
            }
        }
        else
        {
            if(b != 0.0)
            {
                double t = -c / b;
                glm::dvec3 pt = ray.origin + ray.direction * t;
                glm::dvec3 n =  computeNormal(_q, pt);
                reports.push_back(RayHitReport(ray, t, pt, n, _coating));
            }
        }
    }

    bool Quadric::intersects(const Ray& ray)
    {
        double a, b, c;
        params(ray, a, b, c);

        if(a != 0.0)
        {
            return (b*b - 4*a*c) >= 0.0;
        }
        else if(b != 0.0)
        {
            return true;
        }
    }

    void Quadric::params(
            const Ray& ray,
            double& a,
            double& b,
            double& c) const
    {
        glm::dvec4 homoDir = glm::dvec4(ray.direction, 0.0);
        glm::dvec4 homoOrg = glm::dvec4(ray.origin,    1.0);

        a = glm::dot(homoDir, _q * homoDir);
        b = glm::dot(homoDir, _q * homoOrg) * 2.0;
        c = glm::dot(homoOrg, _q * homoOrg);
    }

    void Quadric::setCoating(const std::shared_ptr<Coating>& coating)
    {
        _coating = coating;
    }
}
