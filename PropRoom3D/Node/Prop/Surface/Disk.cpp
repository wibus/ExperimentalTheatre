#include "Disk.h"

#include "../Coating/Coating.h"
#include "Ray/RayHitList.h"
#include "Ray/RayHitReport.h"
#include "Node/Visitor.h"


namespace prop3
{
    Disk::Disk(const glm::dvec3& center,
               const glm::dvec3& normal,
               double radius) :
        _center(center),
        _normal(normal),
        _radius(radius),
        _radius2(radius*radius),
        _d(-glm::dot(normal, center))
    {

    }

    std::shared_ptr<Surface>
        Disk::disk(const glm::dvec3& center,
                   const glm::dvec3& normal,
                   double radius)
    {
        return std::shared_ptr<Surface>(new Disk(center, normal, radius));
    }

    void Disk::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    EPointPosition Disk::isIn(const glm::dvec3& point) const
    {
        return EPointPosition::OUT;
    }

    double Disk::signedDistance(const glm::dvec3& point) const
    {
        return 1.0;
    }

    void Disk::raycast(const Raycast& ray, RayHitList& reports) const
    {
        glm::dvec3 orig = glm::dvec3(_invTransform * glm::dvec4(ray.origin, 1.0));
        glm::dvec3 dir = glm::dvec3(_invTransform * glm::dvec4(ray.direction, 0.0));


        double dirDotNorm = glm::dot(_normal, dir);
        if(dirDotNorm != 0.0)
        {
            double t = -(glm::dot(_normal, orig) + _d) / dirDotNorm;
            if(0.0 < t && t < ray.limit)
            {
                if(glm::distance(orig + dir * t, _center) < _radius)
                {
                    glm::dvec3 pt = ray.origin + ray.direction * t;
                    reports.add(t, pt, _transformN,
                                RayHitReport::NO_TEXCOORD,
                                _coating.get(),
                                _innerMat.get(),
                                _outerMat.get());
                }
            }
        }
    }

    bool Disk::intersects(const Raycast& ray, RayHitList& reports) const
    {
        glm::dvec3 orig = glm::dvec3(_invTransform * glm::dvec4(ray.origin, 1.0));
        glm::dvec3 dir = glm::dvec3(_invTransform * glm::dvec4(ray.direction, 0.0));


        double dirDotNorm = glm::dot(_normal, dir);
        if(dirDotNorm != 0.0)
        {
            double t = -(glm::dot(_normal, orig) + _d) / dirDotNorm;
            if(0.0 < t && t < ray.limit)
            {
                if(glm::distance(orig + dir * t, _center) < _radius)
                    return true;
            }
        }

        return false;
    }

    void Disk::transform(const Transform& transform)
    {
        _transform = transform.mat() * _transform;
        _invTransform = _invTransform * glm::inverse(transform.mat());
        _transformN = glm::transpose(glm::inverse(glm::dmat3(_transform))) * _normal;

        stampCurrentUpdate();
    }
}
