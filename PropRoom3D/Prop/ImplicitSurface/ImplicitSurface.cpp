#include "ImplicitSurface.h"

#include <GLM/gtc/matrix_transform.hpp>

#include "../Ray/RayHitList.h"
#include "../Ray/RayHitReport.h"
#include "../Coating/NoCoating.h"


namespace prop3
{
    Transform::Transform(const glm::dmat4& mat) :
        _mat(mat),
        _isInvComputed(false)
    {

    }

    Transform::Transform(double scale,
                         const glm::dquat& rotation,
                         const glm::dvec3& translation) :
        _mat(),
        _isInvComputed(false)
    {
        _mat[0][0] = scale;
        _mat[1][1] = scale;
        _mat[2][2] = scale;
        _mat = glm::mat4_cast(rotation) * _mat;
        _mat[3] = glm::dvec4(translation, 1);
    }

    const glm::dmat4& Transform::inv() const
    {
        if(_isInvComputed)
            return _inv;

        _isInvComputed = true;
        _inv = glm::inverse(_mat);

        return _inv;
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
        RayHitList&) const
    {
        // Never generates intersection points
    }

    bool SurfaceGhost::intersects(const Ray& ray, RayHitList& reports) const
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
                        RayHitList& reports) const
    {
        _eq->raycast(ray, reports);

        RayHitReport* node = reports.head;
        while(node != nullptr)
        {
            node->normal = -node->normal;
            node = node->_next;
        }
    }

    bool SurfaceInverse::intersects(const Ray& ray, RayHitList& reports) const
    {
        return _eq->intersects(ray, reports);
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
                            RayHitList& reports) const
    {
        raycast(ray, reports, false);
    }

    bool SurfaceOr::intersects(const Ray& ray, RayHitList& reports) const
    {
        return raycast(ray, reports, true);
    }

    bool SurfaceOr::raycast(const Ray& ray,
                           RayHitList& reports,
                           bool isTest) const
    {
        RayHitList eqReports(reports.memoryPool);
        for(const auto& eq : _eqs)
        {
            eqReports.clear();
            eq->raycast(ray, eqReports);

            RayHitReport* parent = nullptr;
            RayHitReport* node = eqReports.head;
            while(node != nullptr)
            {
                bool isIn = false;
                for(const auto& eqTest : _eqs)
                {
                    if(eq.get() != eqTest.get())
                    {
                        if(eqTest->isIn(node->position) ==
                           EPointPosition::IN)
                        {
                            isIn = true;
                            break;
                        }
                    }
                }

                RayHitReport* next = node->_next;

                if(!isIn)
                {
                    if(isTest)
                        return true;

                    if(parent == nullptr)
                        eqReports.head = node->_next;
                    else
                        parent->_next = node->_next;

                    reports.add(node);
                }
                else
                {
                    parent = node;
                }

                node = next;
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
                             RayHitList& reports) const
    {
        raycast(ray, reports, false);
    }

    bool SurfaceAnd::intersects(const Ray& ray, RayHitList& reports) const
    {
        return raycast(ray, reports, true);
    }

    bool SurfaceAnd::raycast(const Ray& ray,
                             RayHitList& reports,
                             bool isTest) const
    {
        RayHitList eqReports(reports.memoryPool);
        for(const auto& eq : _eqs)
        {
            eqReports.clear();
            eq->raycast(ray, eqReports);

            RayHitReport* parent = nullptr;
            RayHitReport* node = eqReports.head;
            while(node != nullptr)
            {
                bool isIn = true;
                for(const auto& eqTest : _eqs)
                {
                    if(eq.get() != eqTest.get())
                    {
                        if(eqTest->isIn(node->position) ==
                           EPointPosition::OUT)
                        {
                            isIn = false;
                            break;
                        }
                    }
                }

                RayHitReport* next = node->_next;

                if(isIn)
                {
                    if(isTest)
                        return true;

                    if(parent == nullptr)
                        eqReports.head = node->_next;
                    else
                        parent->_next = node->_next;

                    reports.add(node);
                }
                else
                {
                    parent = node;
                }

                node = next;
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
