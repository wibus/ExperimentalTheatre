#include "Quadric.h"

#include "../Coating/Coating.h"
#include "Ray/RayHitList.h"
#include "Ray/RayHitReport.h"
#include "../../StageSet/StageSetVisitor.h"


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

        return glm::normalize(glm::dvec3(nx, ny, nz));
    }

    Quadric::Quadric(const glm::dmat4& Q) :
        _q(Q)
    {

    }

    Quadric::Quadric(double A, double E, double H,
                     double B, double C, double D,
                     double F, double G, double I,
                     double J) :
        _q(A, B, C, D,
           B, E, F, G,
           C, F, H, I,
           D, G, I, J)
    {

    }

    std::shared_ptr<Surface>
        Quadric::fromMatrix(const glm::dmat4& Q)
    {
        return std::shared_ptr<Surface>(new Quadric(Q));
    }

    std::shared_ptr<Surface>
        Quadric::fromCoeffs(double A, double E, double H,
                   double B, double C, double D,
                   double F, double G, double I,
                   double J)
    {
        return std::shared_ptr<Surface>(new Quadric(
                A, E, H,
                B, C, D,
                F, G, I,
                J));
    }

    // Ellipsoid : x^2/rx^2 + y^2/ry^2 + z^2/rz^2 = 1
    std::shared_ptr<Surface>
        Quadric::ellipsoid(double rx, double ry, double rz)
    {
        return std::shared_ptr<Surface>(new Quadric(
                1.0 / (rx*rx), // A
                1.0 / (ry*ry), // B
                1.0 / (rz*rz), // C
                0, 0, 0,       // B, C, D
                0, 0, 0,       // F, H, I
                -1));          // J
    }

    // Elliptic cone : x^2/rx^2 + y^2/ry^2 - z^2 = 0
    std::shared_ptr<Surface>
        Quadric::cone(double rx, double ry)
    {
        return std::shared_ptr<Surface>(new Quadric(
                1.0 / (rx*rx), // A
                1.0 / (ry*ry), // B
                -1.0,          // C
                0, 0, 0,       // B, C, D
                0, 0, 0,       // F, H, I
                0));           // J
    }

    // Elliptic paraboloid : x^2/rx^2 + y^2/ry^2 - z = 0
    std::shared_ptr<Surface>
        Quadric::paraboloid(double rx, double ry)
    {
        return std::shared_ptr<Surface>(new Quadric(
                1.0 / (rx*rx), // A
                1.0 / (ry*ry), // B
                0,             // C
                0, 0, 0,      // B, C, D
                0, 0,-1,      // F, H, I
                0));           // J
    }

    // Elliptic cylinder : x^2/rx^2 + y^2/ry^2 = 0
    std::shared_ptr<Surface>
        Quadric::cylinder(double rx, double ry)
    {
        return std::shared_ptr<Surface>(new Quadric(
                1.0 / (rx*rx), // A
                1.0 / (ry*ry), // B
                0,             // C
                0, 0, 0,       // B, C, D
                0, 0, 0,       // F, H, I
                -1));          // J
    }


    void Quadric::transform(const Transform& transform)
    {
        _q = glm::transpose(transform.inv()) * _q * transform.inv();

        stampCurrentUpdate();
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
    void Quadric::raycast(const Raycast& ray, RayHitList& reports) const
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
                    double t = (-b - dsrcSqrt) / (2 * a);
                    if(0.0 < t && t < ray.limit)
                    {
                        glm::dvec3 pt1 = ray.origin + ray.direction*t;
                        glm::dvec3 n1 =  computeNormal(_q, pt1);
                        reports.add(t, ray, pt1, n1,
                                    RayHitReport::NO_TEXCOORD,
                                    _coating.get());
                    }
                }

                {
                    double t = (-b + dsrcSqrt) / (2 * a);
                    if(0.0 < t && t < ray.limit)
                    {
                        glm::dvec3 pt2 = ray.origin + ray.direction*t;
                        glm::dvec3 n2 =  computeNormal(_q, pt2);
                        reports.add(t, ray, pt2, n2,
                                    RayHitReport::NO_TEXCOORD,
                                    _coating.get());
                    }
                }
            }
            else if (dscr == 0.0)
            {
                double t = -b / (2 * a);
                if(0.0 < t && t < ray.limit)
                {
                    glm::dvec3 pt = ray.origin + ray.direction*t;
                    glm::dvec3 n =  computeNormal(_q, pt);
                    reports.add(t, ray, pt, n,
                                RayHitReport::NO_TEXCOORD,
                                _coating.get());
                }
            }
        }
        else
        {
            if(b != 0.0)
            {
                double t = -c / b;
                if(0.0 < t && t < ray.limit)
                {
                    glm::dvec3 pt = ray.origin + ray.direction * t;
                    glm::dvec3 n =  computeNormal(_q, pt);
                    reports.add(t, ray, pt, n,
                                RayHitReport::NO_TEXCOORD,
                                _coating.get());
                }
            }
        }
    }

    bool Quadric::intersects(const Raycast& ray, RayHitList& reports) const
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
            const Raycast& ray,
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

    void Quadric::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
