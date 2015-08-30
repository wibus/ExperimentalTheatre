#include "Surface.h"

#include <GLM/gtc/matrix_transform.hpp>

#include "../Ray/RayHitList.h"
#include "../Ray/RayHitReport.h"
#include "../Coating/NoCoating.h"
#include "../../StageSet/StageSetVisitor.h"


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


    const std::shared_ptr<Coating> Surface::NO_COATING(new NoCoating());

    Surface::Surface()
    {

    }

    Surface::~Surface()
    {

    }


    // SurfaceGhost
    SurfaceGhost::SurfaceGhost(const std::shared_ptr<Surface>& surf) :
        _surf(surf)
    {
    }

    void SurfaceGhost::transform(const Transform& transform)
    {
        _surf->transform(transform);
    }

    EPointPosition SurfaceGhost::isIn(const glm::dvec3& point) const
    {
        return _surf->isIn(point);
    }

    double SurfaceGhost::signedDistance(const glm::dvec3& point) const
    {
        return _surf->signedDistance(point);
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

    void SurfaceGhost::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<StageSetNode>> SurfaceGhost::children() const
    {
        return { _surf };
    }


    // SurfaceNot
    SurfaceInverse::SurfaceInverse(const std::shared_ptr<Surface>& surf) :
        _surf(surf)
    {
    }

    void SurfaceInverse::transform(const Transform& transform)
    {
        _surf->transform(transform);
    }

    EPointPosition SurfaceInverse::isIn(const glm::dvec3& point) const
    {
        EPointPosition pos = _surf->isIn(point);
        return pos != EPointPosition::IN ?
                pos == EPointPosition::OUT ?
                    EPointPosition::IN :
                    EPointPosition::ON :
                    EPointPosition::OUT;
    }

    double SurfaceInverse::signedDistance(const glm::dvec3& point) const
    {
        return -_surf->signedDistance(point);
    }

    void SurfaceInverse::raycast(const Ray& ray,
                        RayHitList& reports) const
    {
        _surf->raycast(ray, reports);

        RayHitReport* node = reports.head;
        while(node != nullptr)
        {
            node->normal = -node->normal;
            node = node->_next;
        }
    }

    bool SurfaceInverse::intersects(const Ray& ray, RayHitList& reports) const
    {
        return _surf->intersects(ray, reports);
    }

    void SurfaceInverse::setCoating(const std::shared_ptr<Coating>& coating)
    {
        _surf->setCoating(coating);
    }

    void SurfaceInverse::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<StageSetNode>> SurfaceInverse::children() const
    {
        return { _surf };
    }


    // SurfaceOr
    SurfaceOr::SurfaceOr(const std::vector<std::shared_ptr<Surface>>& surfs) :
        _surfs(surfs)
    {
    }

    void SurfaceOr::transform(const Transform& transform)
    {
        for(auto& surf : _surfs)
            surf->transform(transform);
    }

    EPointPosition SurfaceOr::isIn(const glm::dvec3& point) const
    {
        EPointPosition pos = EPointPosition::OUT;
        for(const auto& surf : _surfs)
        {
            EPointPosition surfPtPos = surf->isIn(point);
            if(surfPtPos == EPointPosition::IN)
                return EPointPosition::IN;
            else if(surfPtPos == EPointPosition::ON)
                pos = EPointPosition::ON;
        }
        return pos;
    }

    double SurfaceOr::signedDistance(const glm::dvec3& point) const
    {
        static_assert(std::numeric_limits<double>::is_iec559, "IEEE 754 rsurfuired");
        double minDist = std::numeric_limits<double>::infinity();
        for(const auto& surf : _surfs)
        {
            minDist = glm::min(minDist, surf->signedDistance(point));
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
        RayHitList surfReports(reports.memoryPool);
        for(const auto& surf : _surfs)
        {
            surfReports.clear();
            surf->raycast(ray, surfReports);

            RayHitReport* parent = nullptr;
            RayHitReport* node = surfReports.head;
            while(node != nullptr)
            {
                bool isIn = false;
                for(const auto& surfTest : _surfs)
                {
                    if(surf.get() != surfTest.get())
                    {
                        if(surfTest->isIn(node->position) ==
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
                        surfReports.head = node->_next;
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
        for(auto& surf : _surfs)
            surf->setCoating(coating);
    }

    void SurfaceOr::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<StageSetNode>> SurfaceOr::children() const
    {
        return std::vector<std::shared_ptr<StageSetNode>>(_surfs.begin(), _surfs.end());
    }


    // SurfaceAnd
    SurfaceAnd::SurfaceAnd(const std::vector<std::shared_ptr<Surface>>& surfs) :
        _surfs(surfs)
    {
    }

    void SurfaceAnd::transform(const Transform& transform)
    {
        for(auto& surf : _surfs)
            surf->transform(transform);
    }

    EPointPosition SurfaceAnd::isIn(const glm::dvec3& point) const
    {
        EPointPosition pos = EPointPosition::IN;
        for(const auto& surf : _surfs)
        {
            EPointPosition surfPtPos = surf->isIn(point);
            if(surfPtPos == EPointPosition::OUT)
                return EPointPosition::OUT;
            else if(surfPtPos == EPointPosition::ON)
                pos = EPointPosition::ON;
        }
        return pos;
    }

    double SurfaceAnd::signedDistance(const glm::dvec3& point) const
    {
        static_assert(std::numeric_limits<double>::is_iec559, "IEEE 754 rsurfuired");
        double maxDist = -std::numeric_limits<double>::infinity();
        for(const auto& surf : _surfs)
        {
            maxDist = glm::max(maxDist, surf->signedDistance(point));
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
        RayHitList surfReports(reports.memoryPool);
        for(const auto& surf : _surfs)
        {
            surfReports.clear();
            surf->raycast(ray, surfReports);

            RayHitReport* parent = nullptr;
            RayHitReport* node = surfReports.head;
            while(node != nullptr)
            {
                bool isIn = true;
                for(const auto& surfTest : _surfs)
                {
                    if(surf.get() != surfTest.get())
                    {
                        if(surfTest->isIn(node->position) ==
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
                        surfReports.head = node->_next;
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
        for(auto& surf : _surfs)
            surf->setCoating(coating);
    }

    void SurfaceAnd::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<StageSetNode>> SurfaceAnd::children() const
    {
        return std::vector<std::shared_ptr<StageSetNode>>(_surfs.begin(), _surfs.end());
    }


    // Operators
    std::shared_ptr<Surface> operator~ (
            const std::shared_ptr<Surface>& surf)
    {
        return std::shared_ptr<Surface>(new SurfaceGhost(surf));
    }

    std::shared_ptr<Surface> operator!(
            const std::shared_ptr<Surface>& surf)
    {
        return std::shared_ptr<Surface>(new SurfaceInverse(surf));
    }

    std::shared_ptr<Surface> operator| (
            const std::shared_ptr<Surface>& surf1,
            const std::shared_ptr<Surface>& surf2)
    {
        std::shared_ptr<SurfaceOr> cast1;
        if(cast1 = std::dynamic_pointer_cast<SurfaceOr>(surf1))
        {
            cast1->add(surf2);
            return cast1;
        }

        std::shared_ptr<SurfaceOr> cast2;
        if(cast2 = std::dynamic_pointer_cast<SurfaceOr>(surf2))
        {
            cast2->add(surf1);
            return cast2;
        }

        return std::shared_ptr<Surface>(new SurfaceOr({surf1, surf2}));
    }

    std::shared_ptr<Surface> operator& (
            const std::shared_ptr<Surface>& surf1,
            const std::shared_ptr<Surface>& surf2)
    {
        std::shared_ptr<SurfaceAnd> cast1;
        if(cast1 = std::dynamic_pointer_cast<SurfaceAnd>(surf1))
        {
            cast1->add(surf2);
            return cast1;
        }

        std::shared_ptr<SurfaceAnd> cast2;
        if(cast2 = std::dynamic_pointer_cast<SurfaceAnd>(surf2))
        {
            cast2->add(surf1);
            return cast2;
        }

        return std::shared_ptr<Surface>(new SurfaceAnd({surf1, surf2}));
    }
}
