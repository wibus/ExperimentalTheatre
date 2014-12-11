#include "Volume.h"
#include "Prop/Prop.h"

#include <GLM/gtc/matrix_transform.hpp>


namespace prop3
{

    Transform::Transform(const glm::dmat4& transform) :
        _mat(transform),
        _inv(),
        _isInvComputed(false)
    {

    }

    Transform::Transform(double scale,
                         const glm::dquat rotation,
                         const glm::dvec3 translation) :
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

    Volume::Volume()
    {

    }

    Volume::~Volume()
    {

    }

    VolumeNot::VolumeNot(const std::shared_ptr<Volume>& eq) :
        _eq(eq)
    {

    }

    void VolumeNot::transform(const Transform& transform)
    {
        _eq->transform(transform);
    }

    EPointPosition VolumeNot::isIn(const glm::dvec3& point) const
    {
        EPointPosition pos = _eq->isIn(point);
        return pos != EPointPosition::IN ?
                pos == EPointPosition::OUT ?
                    EPointPosition::IN :
                    EPointPosition::ON :
                    EPointPosition::OUT;
    }

    double VolumeNot::signedDistance(const glm::dvec3& point) const
    {
        return -_eq->signedDistance(point);
    }

    void VolumeNot::raycast(const Ray& ray,
                        std::vector<RaycastReport>& reports) const
    {
        _eq->raycast(ray, reports);
        for(RaycastReport& report : reports)
        {
            report.normal = -report.normal;
        }
    }

    VolumeOr::VolumeOr(const std::vector<std::shared_ptr<Volume>>& eqs) :
        _eqs(eqs)
    {
    }

    void VolumeOr::transform(const Transform& transform)
    {
        for(auto& eq : _eqs)
            eq->transform(transform);
    }

    EPointPosition VolumeOr::isIn(const glm::dvec3& point) const
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

    double VolumeOr::signedDistance(const glm::dvec3& point) const
    {
        static_assert(std::numeric_limits<double>::is_iec559, "IEEE 754 required");
        double minDist = std::numeric_limits<double>::infinity();
        for(const auto& eq : _eqs)
        {
            minDist = glm::min(minDist, eq->signedDistance(point));
        }

        return minDist;
    }

    void VolumeOr::raycast(const Ray& ray,
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

    VolumeAnd::VolumeAnd(const std::vector<std::shared_ptr<Volume>>& eqs) :
        _eqs(eqs)
    {
    }

    void VolumeAnd::transform(const Transform& transform)
    {
        for(auto& eq : _eqs)
            eq->transform(transform);
    }

    EPointPosition VolumeAnd::isIn(const glm::dvec3& point) const
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

    double VolumeAnd::signedDistance(const glm::dvec3& point) const
    {
        static_assert(std::numeric_limits<double>::is_iec559, "IEEE 754 required");
        double maxDist = -std::numeric_limits<double>::infinity();
        for(const auto& eq : _eqs)
        {
            maxDist = glm::max(maxDist, eq->signedDistance(point));
        }

        return maxDist;
    }

    void VolumeAnd::raycast(const Ray& ray,
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


    std::shared_ptr<Volume> operator!(
            const std::shared_ptr<Volume>& eq)
    {
        return std::shared_ptr<Volume>(new VolumeNot(eq));
    }

    std::shared_ptr<Volume> operator| (
            const std::shared_ptr<Volume>& eq1,
            const std::shared_ptr<Volume>& eq2)
    {
        std::shared_ptr<VolumeOr> cast1;
        if(cast1 = std::dynamic_pointer_cast<VolumeOr>(eq1))
        {
            cast1->addVolume(eq2);
            return cast1;
        }

        std::shared_ptr<VolumeOr> cast2;
        if(cast2 = std::dynamic_pointer_cast<VolumeOr>(eq2))
        {
            cast2->addVolume(eq1);
            return cast2;
        }

        return std::shared_ptr<Volume>(new VolumeOr({eq1, eq2}));
    }

    std::shared_ptr<Volume> operator& (
            const std::shared_ptr<Volume>& eq1,
            const std::shared_ptr<Volume>& eq2)
    {
        std::shared_ptr<VolumeAnd> cast1;
        if(cast1 = std::dynamic_pointer_cast<VolumeAnd>(eq1))
        {
            cast1->addVolume(eq2);
            return cast1;
        }

        std::shared_ptr<VolumeAnd> cast2;
        if(cast2 = std::dynamic_pointer_cast<VolumeAnd>(eq2))
        {
            cast2->addVolume(eq1);
            return cast2;
        }

        return std::shared_ptr<Volume>(new VolumeAnd({eq1, eq2}));
    }
}
