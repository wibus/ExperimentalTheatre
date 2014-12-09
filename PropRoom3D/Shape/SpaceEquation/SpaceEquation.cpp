#include "SpaceEquation.h"
#include "Shape/AbstractShape.h"


namespace prop3
{
    EqNot::EqNot(const std::shared_ptr<SpaceEquation>& eq) :
        _eq(eq)
    {

    }

    bool EqNot::isIn(const glm::dvec3& point) const
    {
        return !_eq->isIn(point);
    }

    double EqNot::computeSignedDistance(const glm::dvec3& point) const
    {
        return -_eq->computeSignedDistance(point);
    }

    void EqNot::raycast(const Ray& ray,
                        std::vector<RaycastReport>& reports) const
    {
        _eq->raycast(ray, reports);
        for(RaycastReport& report : reports)
        {
            report.normal = -report.normal;
        }
    }

    EqOr::EqOr(const std::shared_ptr<SpaceEquation>& eq1,
               const std::shared_ptr<SpaceEquation>& eq2) :
        _eqs({eq1, eq2})
    {
    }

    EqOr::EqOr(const std::vector<std::shared_ptr<SpaceEquation>>& eqs) :
        _eqs(eqs)
    {
    }

    bool EqOr::isIn(const glm::dvec3& point) const
    {
        for(const auto& eq : _eqs)
            if(eq->isIn(point))
                return true;
        return false;
    }

    double EqOr::computeSignedDistance(const glm::dvec3& point) const
    {
        static_assert(std::numeric_limits<double>::is_iec559, "IEEE 754 required");
        double minDist = std::numeric_limits<double>::infinity();
        for(const auto& eq : _eqs)
        {
            minDist = glm::min(minDist, eq->computeSignedDistance(point));
        }

        return minDist;
    }

    void EqOr::raycast(const Ray& ray,
                       std::vector<RaycastReport>& reports) const
    {
        std::vector<RaycastReport> eqReports;
        for(const auto& eq : _eqs)
        {
            eqReports.clear();
            eq->raycast(ray, eqReports);

            for(RaycastReport& report : eqReports)
            {
                bool isIn = false;
                for(const auto& eqTest : _eqs)
                {
                    if(eq.get() != eqTest.get())
                    {
                        if(eqTest->isIn(report.position))
                        {
                            isIn = true;
                            break;
                        }
                    }
                }

                if(!isIn)
                {
                    reports.push_back(report);
                }
            }
        }
    }

    EqAnd::EqAnd(const std::shared_ptr<SpaceEquation>& eq1,
                 const std::shared_ptr<SpaceEquation>& eq2) :
        _eqs({eq1, eq2})
    {

    }

    EqAnd::EqAnd(const std::vector<std::shared_ptr<SpaceEquation>>& eqs) :
        _eqs(eqs)
    {
        _eqs = eqs;
    }

    bool EqAnd::isIn(const glm::dvec3& point) const
    {
        for(const auto& eq : _eqs)
            if(!eq->isIn(point))
                return false;
        return true;
    }

    double EqAnd::computeSignedDistance(const glm::dvec3& point) const
    {
        static_assert(std::numeric_limits<double>::is_iec559, "IEEE 754 required");
        double maxDist = -std::numeric_limits<double>::infinity();
        for(const auto& eq : _eqs)
        {
            maxDist = glm::max(maxDist, eq->computeSignedDistance(point));
        }

        return maxDist;
    }

    void EqAnd::raycast(const Ray& ray,
                        std::vector<RaycastReport>& reports) const
    {
        std::vector<RaycastReport> eqReports;
        for(const auto& eq : _eqs)
        {
            eqReports.clear();
            eq->raycast(ray, eqReports);

            for(RaycastReport& report : eqReports)
            {
                bool isIn = true;
                for(const auto& eqTest : _eqs)
                {
                    if(eq.get() != eqTest.get())
                    {
                        if(!eqTest->isIn(report.position))
                        {
                            isIn = false;
                            break;
                        }
                    }
                }

                if(isIn)
                {
                    reports.push_back(report);
                }
            }
        }
    }
}
