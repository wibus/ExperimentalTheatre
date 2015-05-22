#include "ImplicitSurface.h"

#include <GLM/gtc/matrix_transform.hpp>

#include "../Ray/RayHitReport.h"
#include "../Coating/NoCoating.h"


namespace prop3
{
    Transform::Transform(double scale,
                         const glm::dquat& rotation,
                         const glm::dvec3& translation) :
        _mat(1.0),
        _inv(1.0),
        _isInvComputed(true)
    {
        _mat[0][0] = scale;
        _mat[1][1] = scale;
        _mat[2][2] = scale;
        _mat = glm::mat4_cast(rotation) * _mat;
        _mat[0] = glm::dvec4(translation, 1);

        _inv[0] = glm::dvec4(-translation, 1);
        _inv = glm::mat4_cast(glm::conjugate(rotation)) * _mat;
        glm::dmat4 invScale;
        invScale[0][0] = 1.0 / scale;
        invScale[1][1] = 1.0 / scale;
        invScale[2][2] = 1.0 / scale;
        _inv = invScale * _inv;
    }

    const std::shared_ptr<Coating> ImplicitSurface::NO_COATING(new NoCoating());

    ImplicitSurface::ImplicitSurface()
    {

    }

    ImplicitSurface::~ImplicitSurface()
    {

    }


    // SurfaceGhost
    SurfaceGhost::SurfaceGhost(const std::shared_ptr<ImplicitSurface>& eq) :
        _eq(eq)
    {
    }

    void SurfaceGhost::transform(const Transform& transform)
    {
        _eq->transform(transform);
    }

    EPointPosition SurfaceGhost::isIn(const glm::dvec3& point) const
    {
        return _eq->isIn(point);
    }

    double SurfaceGhost::signedDistance(const glm::dvec3& point) const
    {
        return _eq->signedDistance(point);
    }

    void SurfaceGhost::raycast(const Ray&,
        std::vector<RayHitReport>&) const
    {
        // Never generates intersection points
    }

    bool SurfaceGhost::intersects(const Ray& ray)
    {
        return false;
    }

    void SurfaceGhost::setCoating(const std::shared_ptr<Coating>& coating)
    {
    }


    // SurfaceNot
    SurfaceInverse::SurfaceInverse(const std::shared_ptr<ImplicitSurface>& eq) :
        _eq(eq)
    {
    }

    void SurfaceInverse::transform(const Transform& transform)
    {
        _eq->transform(transform);
    }

    EPointPosition SurfaceInverse::isIn(const glm::dvec3& point) const
    {
        EPointPosition pos = _eq->isIn(point);
        return pos != EPointPosition::IN ?
                pos == EPointPosition::OUT ?
                    EPointPosition::IN :
                    EPointPosition::ON :
                    EPointPosition::OUT;
    }

    double SurfaceInverse::signedDistance(const glm::dvec3& point) const
    {
        return -_eq->signedDistance(point);
    }

    void SurfaceInverse::raycast(const Ray& ray,
                        std::vector<RayHitReport>& reports) const
    {
        _eq->raycast(ray, reports);
        for(RayHitReport& report : reports)
        {
            report.normal = -report.normal;
        }
    }

    bool SurfaceInverse::intersects(const Ray& ray)
    {
        return _eq->intersects(ray);
    }

    void SurfaceInverse::setCoating(const std::shared_ptr<Coating>& coating)
    {
        _eq->setCoating(coating);
    }


    // SurfaceOr
    SurfaceOr::SurfaceOr(const std::vector<std::shared_ptr<ImplicitSurface>>& eqs) :
        _eqs(eqs)
    {
    }

    void SurfaceOr::transform(const Transform& transform)
    {
        for(auto& eq : _eqs)
            eq->transform(transform);
    }

    EPointPosition SurfaceOr::isIn(const glm::dvec3& point) const
    {
        EPointPosition pos = EPointPosition::OUT;
        for(const auto& eq : _eqs)
        {
            EPointPosition eqPtPos = eq->isIn(point);
            if(eqPtPos == EPointPosition::IN)
                return EPointPosition::IN;
            else if(eqPtPos == EPointPosition::ON)
                pos = EPointPosition::ON;
        }
        return pos;
    }

    double SurfaceOr::signedDistance(const glm::dvec3& point) const
    {
        static_assert(std::numeric_limits<double>::is_iec559, "IEEE 754 required");
        double minDist = std::numeric_limits<double>::infinity();
        for(const auto& eq : _eqs)
        {
            minDist = glm::min(minDist, eq->signedDistance(point));
        }

        return minDist;
    }

    void SurfaceOr::raycast(const Ray& ray,
                           std::vector<RayHitReport>& reports) const
    {
        raycast(ray, reports, false);
    }

    bool SurfaceOr::intersects(const Ray& ray)
    {
        std::vector<RayHitReport> reports;
        return raycast(ray, reports, true);
    }

    bool SurfaceOr::raycast(const Ray& ray,
                           std::vector<RayHitReport>& reports,
                           bool isTest) const
    {
        std::vector<RayHitReport> eqReports;
        for(const auto& eq : _eqs)
        {
            eqReports.clear();
            eq->raycast(ray, eqReports);

            for(RayHitReport& report : eqReports)
            {
                bool isIn = false;
                for(const auto& eqTest : _eqs)
                {
                    if(eq.get() != eqTest.get())
                    {
                        if(eqTest->isIn(report.position) ==
                           EPointPosition::IN)
                        {
                            isIn = true;
                            break;
                        }
                    }
                }

                if(!isIn)
                {
                    if(isTest)
                        return true;

                    reports.push_back(report);
                }
            }
        }

        return false;
    }

    void SurfaceOr::setCoating(const std::shared_ptr<Coating>& coating)
    {
        for(auto& eq : _eqs)
            eq->setCoating(coating);
    }


    // SurfaceAnd
    SurfaceAnd::SurfaceAnd(const std::vector<std::shared_ptr<ImplicitSurface>>& eqs) :
        _eqs(eqs)
    {
    }

    void SurfaceAnd::transform(const Transform& transform)
    {
        for(auto& eq : _eqs)
            eq->transform(transform);
    }

    EPointPosition SurfaceAnd::isIn(const glm::dvec3& point) const
    {
        EPointPosition pos = EPointPosition::IN;
        for(const auto& eq : _eqs)
        {
            EPointPosition eqPtPos = eq->isIn(point);
            if(eqPtPos == EPointPosition::OUT)
                return EPointPosition::OUT;
            else if(eqPtPos == EPointPosition::ON)
                pos = EPointPosition::ON;
        }
        return pos;
    }

    double SurfaceAnd::signedDistance(const glm::dvec3& point) const
    {
        static_assert(std::numeric_limits<double>::is_iec559, "IEEE 754 required");
        double maxDist = -std::numeric_limits<double>::infinity();
        for(const auto& eq : _eqs)
        {
            maxDist = glm::max(maxDist, eq->signedDistance(point));
        }

        return maxDist;
    }

    void SurfaceAnd::raycast(const Ray& ray,
                           std::vector<RayHitReport>& reports) const
    {
        raycast(ray, reports, false);
    }

    bool SurfaceAnd::intersects(const Ray& ray)
    {
        std::vector<RayHitReport> reports;
        return raycast(ray, reports, true);
    }

    bool SurfaceAnd::raycast(const Ray& ray,
                            std::vector<RayHitReport>& reports,
                            bool isTest) const
    {
        std::vector<RayHitReport> eqReports;
        for(const auto& eq : _eqs)
        {
            eqReports.clear();
            eq->raycast(ray, eqReports);

            for(RayHitReport& report : eqReports)
            {
                bool isIn = true;
                for(const auto& eqTest : _eqs)
                {
                    if(eq.get() != eqTest.get())
                    {
                        if(eqTest->isIn(report.position) ==
                           EPointPosition::OUT)
                        {
                            isIn = false;
                            break;
                        }
                    }
                }

                if(isIn)
                {
                    if(isTest)
                        return true;

                    reports.push_back(report);
                }
            }
        }

        return false;
    }

    void SurfaceAnd::setCoating(const std::shared_ptr<Coating>& coating)
    {
        for(auto& eq : _eqs)
            eq->setCoating(coating);
    }


    // Operators
    std::shared_ptr<ImplicitSurface> operator~ (
            const std::shared_ptr<ImplicitSurface>& eq)
    {
        return std::shared_ptr<ImplicitSurface>(new SurfaceGhost(eq));
    }

    std::shared_ptr<ImplicitSurface> operator!(
            const std::shared_ptr<ImplicitSurface>& eq)
    {
        return std::shared_ptr<ImplicitSurface>(new SurfaceInverse(eq));
    }

    std::shared_ptr<ImplicitSurface> operator| (
            const std::shared_ptr<ImplicitSurface>& eq1,
            const std::shared_ptr<ImplicitSurface>& eq2)
    {
        std::shared_ptr<SurfaceOr> cast1;
        if(cast1 = std::dynamic_pointer_cast<SurfaceOr>(eq1))
        {
            cast1->add(eq2);
            return cast1;
        }

        std::shared_ptr<SurfaceOr> cast2;
        if(cast2 = std::dynamic_pointer_cast<SurfaceOr>(eq2))
        {
            cast2->add(eq1);
            return cast2;
        }

        return std::shared_ptr<ImplicitSurface>(new SurfaceOr({eq1, eq2}));
    }

    std::shared_ptr<ImplicitSurface> operator& (
            const std::shared_ptr<ImplicitSurface>& eq1,
            const std::shared_ptr<ImplicitSurface>& eq2)
    {
        std::shared_ptr<SurfaceAnd> cast1;
        if(cast1 = std::dynamic_pointer_cast<SurfaceAnd>(eq1))
        {
            cast1->add(eq2);
            return cast1;
        }

        std::shared_ptr<SurfaceAnd> cast2;
        if(cast2 = std::dynamic_pointer_cast<SurfaceAnd>(eq2))
        {
            cast2->add(eq1);
            return cast2;
        }

        return std::shared_ptr<ImplicitSurface>(new SurfaceAnd({eq1, eq2}));
    }
}
