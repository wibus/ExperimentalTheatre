#include "Plane.h"

#include "../Coating/Coating.h"
#include "Ray/RayHitList.h"
#include "Ray/RayHitReport.h"
#include "Node/Visitor.h"


namespace prop3
{
    Plane::Plane(const glm::dvec3& normal, const glm::dvec3& origin) :
        _normal(glm::normalize(normal)),
        _d(-glm::dot(normal, origin))
    {
    }

    Plane::Plane(const glm::dvec4& representation) :
        _normal(representation),
        _d(representation.w)
    {
    }

    Plane::Plane(double a, double b, double c, double d) :
        _normal(a, b, c),
        _d(d)
    {
    }

    std::shared_ptr<Surface>
        Plane::plane(double a, double b, double c, double d)
    {
        return std::shared_ptr<Surface>(new Plane(a, b, c, d));
    }

    std::shared_ptr<Surface>
        Plane::plane(const glm::dvec4& representation)
    {
        return std::shared_ptr<Surface>(new Plane(representation));
    }

    std::shared_ptr<Surface>
        Plane::plane(const glm::dvec3& normal, const glm::dvec3& origin)
    {
        return std::shared_ptr<Surface>(new Plane(normal, origin));
    }

    void Plane::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    EPointPosition Plane::isIn(const glm::dvec3& point) const
    {
        double dist = glm::dot(_normal, point) + _d;
        return dist < 0 ? EPointPosition::IN :
                    dist > 0.0 ? EPointPosition::OUT :
                        EPointPosition::ON;
    }

    double Plane::signedDistance(const glm::dvec3& point) const
    {
        return glm::dot(_normal, point) + _d;
    }

    void Plane::raycast(const Raycast& ray, RayHitList& reports) const
    {
        double dirDotNorm = glm::dot(_normal, ray.direction);
        if(dirDotNorm != 0.0)
        {
            double t = -(glm::dot(_normal, ray.origin) + _d) / dirDotNorm;
            if(0.0 < t && t < ray.limit)
            {
                glm::dvec3 pt = ray.origin + ray.direction * t;
                reports.add(t, pt, _normal,
                            RayHitReport::NO_TEXCOORD,
                            _coating.get(),
                            _innerMat.get(),
                            _outerMat.get());
            }
        }
    }

    bool Plane::intersects(const Raycast& ray, RayHitList& reports) const
    {
        return glm::dot(ray.direction, _normal) != 0.0;
    }

    void Plane::transform(const Transform& transform)
    {
        glm::dvec3 pt = glm::dvec3(transform.mat() * glm::dvec4((-_d) * _normal, 1.0));
        _normal = glm::normalize(glm::dvec3(transform.mat() * glm::dvec4(_normal, 0.0)));
        _d = -glm::dot(_normal, pt);

        stampCurrentUpdate();
    }


    // Textures
    PlaneTexture::PlaneTexture(const glm::dvec3& normal, const glm::dvec3& origin,
            const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin) :
        Plane(normal, origin),
        _texOrigin(texOrigin),
        _texU(texU), _texV(texV)
    {

    }

    PlaneTexture::PlaneTexture(const glm::dvec4& representation,
            const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin) :
        Plane(representation),
        _texOrigin(texOrigin),
        _texU(texU), _texV(texV)
    {

    }


    PlaneTexture::PlaneTexture(double a, double b, double c, double d,
            const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin) :
        Plane(a, b, c, d),
        _texOrigin(texOrigin),
        _texU(texU), _texV(texV)
    {

    }

    std::shared_ptr<Surface>
        PlaneTexture::plane(double a, double b, double c, double d,
                            const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin)
    {
        return std::shared_ptr<Surface>(
                    new PlaneTexture(a, b, c, d, texU, texV, texOrigin));
    }

    std::shared_ptr<Surface>
        PlaneTexture::plane(const glm::dvec4& representation,
                            const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin)
    {
        return std::shared_ptr<Surface>(
                    new PlaneTexture(representation, texU, texV, texOrigin));
    }

    std::shared_ptr<Surface>
        PlaneTexture::plane(const glm::dvec3& normal, const glm::dvec3& origin,
                            const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin)
    {
        return std::shared_ptr<Surface>(
                    new PlaneTexture(normal, origin, texU, texV, texOrigin));
    }

    void PlaneTexture::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    void PlaneTexture::raycast(const Raycast& ray, RayHitList& reports) const
    {
        RayHitReport* last = reports.head;
        Plane::raycast(ray, reports);
        RayHitReport* node = reports.head;

        while(node != last)
        {
            RayHitReport& r = *node;
            glm::dvec3 dist = r.position - _texOrigin;
            r.texCoord.s = glm::dot(dist, _texU) / glm::dot(_texU, _texU);
            r.texCoord.t = glm::dot(dist, _texV) / glm::dot(_texV, _texV);

            node = node->_next;
        }
    }

    void PlaneTexture::transform(const Transform& transform)
    {
        _texOrigin = glm::dvec3(transform.mat() * glm::dvec4(_texOrigin, 1.0));
        _texU = glm::dvec3(transform.mat() * glm::dvec4(_texU, 0.0));
        _texV = glm::dvec3(transform.mat() * glm::dvec4(_texV, 0.0));

        Plane::transform(transform);
    }
}
