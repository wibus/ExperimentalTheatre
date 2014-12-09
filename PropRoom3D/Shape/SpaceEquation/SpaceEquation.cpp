#include "SpaceEquation.h"
#include "Shape/AbstractShape.h"


namespace prop3
{
    SpaceEquation::SpaceEquation()
    {

    }

    SpaceEquation::~SpaceEquation()
    {

    }

    EqNot::EqNot(const std::shared_ptr<SpaceEquation>& eq) :
        _eq(eq)
    {

    }

    EPointPosition EqNot::isIn(const glm::dvec3& point) const
    {
        EPointPosition pos = _eq->isIn(point);
        return pos != EPointPosition::IN ?
                pos == EPointPosition::OUT ?
                    EPointPosition::IN :
                    EPointPosition::ON :
                    EPointPosition::OUT;
    }

    double EqNot::signedDistance(const glm::dvec3& point) const
    {
        return -_eq->signedDistance(point);
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

    EPointPosition EqOr::isIn(const glm::dvec3& point) const
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

    double EqOr::signedDistance(const glm::dvec3& point) const
    {
        static_assert(std::numeric_limits<double>::is_iec559, "IEEE 754 required");
        double minDist = std::numeric_limits<double>::infinity();
        for(const auto& eq : _eqs)
        {
            minDist = glm::min(minDist, eq->signedDistance(point));
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

    EPointPosition EqAnd::isIn(const glm::dvec3& point) const
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

    double EqAnd::signedDistance(const glm::dvec3& point) const
    {
        static_assert(std::numeric_limits<double>::is_iec559, "IEEE 754 required");
        double maxDist = -std::numeric_limits<double>::infinity();
        for(const auto& eq : _eqs)
        {
            maxDist = glm::max(maxDist, eq->signedDistance(point));
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
                    reports.push_back(report);
                }
            }
        }
    }
}
