#include "Surface.h"

#include <GLM/gtc/matrix_transform.hpp>

#include "Ray/RayHitList.h"
#include "Ray/RayHitReport.h"
#include "../Material/UniformStdMaterial.h"
#include "../Coating/UniformStdCoating.h"
#include "Serial/Visitor.h"


namespace prop3
{
    Transform::Transform(const glm::dmat4& mat) :
        _mat(mat),
        _isInvComputed(false)
    {

    }

    const glm::dmat4& Transform::inv() const
    {
        if(_isInvComputed)
            return _inv;

        _isInvComputed = true;
        _inv = glm::inverse(_mat);

        return _inv;
    }

    void applyTransformation(const std::shared_ptr<Surface>& surf,
                             const Transform& transform)
    {
        surf->transform(transform);
    }


    const std::shared_ptr<Coating>  Surface::NO_COATING(new UniformStdCoating());
    const std::shared_ptr<Material> Surface::DEFAULT_MATERIAL(material::createInsulator(color::white, 1.40, 1.0, 1.0));
    const std::shared_ptr<Material> Surface::ENVIRONMENT_MATERIAL(material::createInsulator(color::white, 1.0, 0.0, 0.0));

    Surface::Surface()
    {

    }

    Surface::~Surface()
    {

    }

    std::shared_ptr<Surface> Surface::shell(
            const std::shared_ptr<Surface>& surf)
    {
        return std::shared_ptr<Surface>(new SurfaceShell(surf));
    }

    std::shared_ptr<Surface> Surface::transform(std::shared_ptr<Surface>& surf, const glm::dmat4& mat)
    {
        if(!surf->isAffineTransformable())
            surf = shell(surf);
        surf->transform(mat);
        return surf;
    }

    std::shared_ptr<Surface> Surface::translate(std::shared_ptr<Surface>& surf, const glm::dvec3& dis)
    {
        if(!surf->isTranslatable())
            surf = shell(surf);
        surf->transform(glm::translate(glm::dmat4(), dis));
        return surf;
    }

    std::shared_ptr<Surface> Surface::rotate(std::shared_ptr<Surface>& surf, double angle, const glm::dvec3& axis)
    {
        if(!surf->isRotatable())
            surf = shell(surf);
        surf->transform(glm::rotate(glm::dmat4(), angle, axis));
        return surf;
    }

    std::shared_ptr<Surface> Surface::scale(std::shared_ptr<Surface>& surf, double coeff)
    {
        if(!surf->isRotatable())
            surf = shell(surf);
        surf->transform(glm::scale(glm::dmat4(), glm::dvec3(coeff)));
        return surf;
    }


    // Physical surfaces
    PhysicalSurface::PhysicalSurface() :
        _coating(NO_COATING),
        _innerMat(DEFAULT_MATERIAL),
        _outerMat(ENVIRONMENT_MATERIAL)
    {

    }

    std::vector<std::shared_ptr<Node>> PhysicalSurface::children() const
    {
        return { _coating, _innerMat, _outerMat };
    }

    void PhysicalSurface::setCoating(const std::shared_ptr<Coating>& coating)
    {
        _coating = coating;

        stampCurrentUpdate();
    }

    void PhysicalSurface::setInnerMaterial(const std::shared_ptr<Material>& mat)
    {
        _innerMat = mat;

        stampCurrentUpdate();
    }

    void PhysicalSurface::setOuterMaterial(const std::shared_ptr<Material>& mat)
    {
        _outerMat = mat;

        stampCurrentUpdate();
    }


    // Surface Shell
    SurfaceShell::SurfaceShell(const std::shared_ptr<Surface>& surf) :
        _surf(surf),
        _invTransform()
    {
        // When _coating == nullptr,
        // use child surface's coating
        _coating.reset();

        // When inner (outer) material is nullptr
        // don't exchange with child surface's material
        _innerMat.reset();
        _outerMat.reset();
    }

    void SurfaceShell::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<Node>> SurfaceShell::children() const
    {
        std::vector<std::shared_ptr<Node>> c{_surf};
        if(_coating.get() != nullptr) c.push_back( _coating );
        if(_innerMat.get() != nullptr) c.push_back( _innerMat );
        if(_outerMat.get() != nullptr) c.push_back( _outerMat );
        return c;
    }

    EPointPosition SurfaceShell::isIn(const glm::dvec3& point) const
    {
        return _surf->isIn(glm::dvec3(_invTransform * glm::dvec4(point, 1.0)));
    }

    double SurfaceShell::signedDistance(const glm::dvec3& point) const
    {
        return _surf->signedDistance(glm::dvec3(_invTransform * glm::dvec4(point, 1.0)));
    }

    void SurfaceShell::raycast(const Raycast& ray, RayHitList& reports) const
    {
        Raycast tRay = ray;
        tRay.origin = glm::dvec3(_invTransform * glm::dvec4(ray.origin, 1.0));
        tRay.direction = glm::dvec3(_invTransform * glm::dvec4(ray.direction, 0.0));

        RayHitReport* last = reports.head;
        _surf->raycast(tRay, reports);
        RayHitReport* first = reports.head;

        while(first != last)
        {
            RayHitReport& r = *first;
            r.position = glm::dvec3(_transform * glm::dvec4(r.position, 1.0));
            r.normal = glm::dvec3(_transform * glm::dvec4(r.normal, 0.0));
            r.normal  = glm::normalize(r.normal);
            r.incidentRay = ray;

            if(_coating.get() != nullptr)
                r.coating = _coating.get();
            if(_innerMat.get() != nullptr)
                r.innerMat = _innerMat.get();
            if(_outerMat.get() != nullptr)
                r.outerMat = _outerMat.get();

            first = first->_next;
        }
    }

    bool SurfaceShell::intersects(const Raycast& ray, RayHitList& reports) const
    {
        Raycast tRay = ray;
        tRay.origin = glm::dvec3(_invTransform * glm::dvec4(ray.origin, 1.0));
        tRay.direction = glm::dvec3(_invTransform * glm::dvec4(ray.direction, 0.0));
        return _surf->intersects(tRay, reports);
    }

    void SurfaceShell::transform(const Transform& transform)
    {
        _transform = transform.mat() * _transform;
        _invTransform = _invTransform * transform.inv();

        stampCurrentUpdate();
    }


    // SurfaceGhost
    SurfaceGhost::SurfaceGhost(const std::shared_ptr<Surface>& surf) :
        _surf(surf)
    {
    }

    void SurfaceGhost::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<Node>> SurfaceGhost::children() const
    {
        return { _surf };
    }

    EPointPosition SurfaceGhost::isIn(const glm::dvec3& point) const
    {
        return _surf->isIn(point);
    }

    double SurfaceGhost::signedDistance(const glm::dvec3& point) const
    {
        return _surf->signedDistance(point);
    }

    void SurfaceGhost::raycast(const Raycast&, RayHitList&) const
    {
        // Never generates intersection points
    }

    bool SurfaceGhost::intersects(const Raycast&, RayHitList&) const
    {
        return false;
    }

    void SurfaceGhost::setCoating(const std::shared_ptr<Coating>&)
    {
        // No intersection, no coating
    }

    void SurfaceGhost::setInnerMaterial(const std::shared_ptr<Material>&)
    {
        // No intersection, no material interface
    }

    void SurfaceGhost::setOuterMaterial(const std::shared_ptr<Material>&)
    {
        // No intersection, no material interface
    }

    bool SurfaceGhost::isAffineTransformable() const
    {
        return _surf->isAffineTransformable();
    }

    bool SurfaceGhost::isTranslatable() const
    {
        return _surf->isTranslatable();
    }

    bool SurfaceGhost::isRotatable() const
    {
        return _surf->isRotatable();
    }

    bool SurfaceGhost::isScalable() const
    {
        return _surf->isScalable();
    }

    void SurfaceGhost::transform(const Transform& transform)
    {
        applyTransformation(_surf, transform);
    }


    // SurfaceNot
    SurfaceInverse::SurfaceInverse(const std::shared_ptr<Surface>& surf) :
        _surf(surf)
    {
    }

    void SurfaceInverse::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<Node>> SurfaceInverse::children() const
    {
        return { _surf };
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

    void SurfaceInverse::raycast(
            const Raycast& ray,
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

    bool SurfaceInverse::intersects(const Raycast& ray, RayHitList& reports) const
    {
        return _surf->intersects(ray, reports);
    }

    void SurfaceInverse::setCoating(const std::shared_ptr<Coating>& coating)
    {
        _surf->setCoating(coating);
    }

    void SurfaceInverse::setInnerMaterial(const std::shared_ptr<Material>& mat)
    {
        _surf->setInnerMaterial(mat);
    }

    void SurfaceInverse::setOuterMaterial(const std::shared_ptr<Material>& mat)
    {
        _surf->setOuterMaterial(mat);
    }

    bool SurfaceInverse::isAffineTransformable() const
    {
        return _surf->isAffineTransformable();
    }

    bool SurfaceInverse::isTranslatable() const
    {
        return _surf->isTranslatable();
    }

    bool SurfaceInverse::isRotatable() const
    {
        return _surf->isRotatable();
    }

    bool SurfaceInverse::isScalable() const
    {
        return _surf->isScalable();
    }

    void SurfaceInverse::transform(const Transform& transform)
    {
        applyTransformation(_surf, transform);
    }


    // SurfaceOr
    SurfaceOr::SurfaceOr(const std::vector<std::shared_ptr<Surface>>& surfs) :
        _surfs(surfs)
    {
    }

    void SurfaceOr::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<Node>> SurfaceOr::children() const
    {
        return std::vector<std::shared_ptr<Node>>(_surfs.begin(), _surfs.end());
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

    void SurfaceOr::raycast(const Raycast& ray,
                            RayHitList& reports) const
    {
        raycast(ray, reports, false);
    }

    bool SurfaceOr::intersects(const Raycast& ray, RayHitList& reports) const
    {
        return raycast(ray, reports, true);
    }

    bool SurfaceOr::raycast(const Raycast& ray,
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

    void SurfaceOr::setInnerMaterial(const std::shared_ptr<Material>& mat)
    {
        for(auto& surf : _surfs)
            surf->setInnerMaterial(mat);
    }

    void SurfaceOr::setOuterMaterial(const std::shared_ptr<Material>& mat)
    {
        for(auto& surf : _surfs)
            surf->setOuterMaterial(mat);
    }

    bool SurfaceOr::isAffineTransformable() const
    {
        for(const auto& s : _surfs)
            if(!s->isAffineTransformable())
                return false;
        return true;
    }

    bool SurfaceOr::isTranslatable() const
    {
        for(const auto& s : _surfs)
            if(!s->isTranslatable())
                return false;
        return true;
    }

    bool SurfaceOr::isRotatable() const
    {
        for(const auto& s : _surfs)
            if(!s->isRotatable())
                return false;
        return true;
    }

    bool SurfaceOr::isScalable() const
    {
        for(const auto& s : _surfs)
            if(!s->isScalable())
                return false;
        return true;
    }

    void SurfaceOr::transform(const Transform& transform)
    {
        for(auto& surf : _surfs)
            applyTransformation(surf, transform);
    }


    // SurfaceAnd
    SurfaceAnd::SurfaceAnd(const std::vector<std::shared_ptr<Surface>>& surfs) :
        _surfs(surfs)
    {
    }

    void SurfaceAnd::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<Node>> SurfaceAnd::children() const
    {
        return std::vector<std::shared_ptr<Node>>(_surfs.begin(), _surfs.end());
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

    void SurfaceAnd::raycast(const Raycast& ray,
                             RayHitList& reports) const
    {
        raycast(ray, reports, false);
    }

    bool SurfaceAnd::intersects(const Raycast& ray, RayHitList& reports) const
    {
        return raycast(ray, reports, true);
    }

    bool SurfaceAnd::raycast(const Raycast& ray,
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

    void SurfaceAnd::setInnerMaterial(const std::shared_ptr<Material>& mat)
    {
        for(auto& surf : _surfs)
            surf->setInnerMaterial(mat);
    }

    void SurfaceAnd::setOuterMaterial(const std::shared_ptr<Material>& mat)
    {
        for(auto& surf : _surfs)
            surf->setOuterMaterial(mat);
    }

    bool SurfaceAnd::isAffineTransformable() const
    {
        for(const auto& s : _surfs)
            if(!s->isAffineTransformable())
                return false;
        return true;
    }

    bool SurfaceAnd::isTranslatable() const
    {
        for(const auto& s : _surfs)
            if(!s->isTranslatable())
                return false;
        return true;
    }

    bool SurfaceAnd::isRotatable() const
    {
        for(const auto& s : _surfs)
            if(!s->isRotatable())
                return false;
        return true;
    }

    bool SurfaceAnd::isScalable() const
    {
        for(const auto& s : _surfs)
            if(!s->isScalable())
                return false;
        return true;
    }

    void SurfaceAnd::transform(const Transform& transform)
    {
        for(auto& surf : _surfs)
            applyTransformation(surf, transform);
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

    std::shared_ptr<Surface> operator^ (
            const std::shared_ptr<Surface>& surf1,
            const std::shared_ptr<Surface>& surf2)
    {
        return Surface::shell(surf1 & ~surf2);
    }
}
