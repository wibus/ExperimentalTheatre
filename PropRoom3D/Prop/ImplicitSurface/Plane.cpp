#include "Plane.h"

#include "../Coating/Coating.h"
#include "../Ray/RayHitList.h"
#include "../Ray/RayHitReport.h"
#include "../../Scene/SceneVisitor.h"


namespace prop3
{
    Plane::Plane(const glm::dvec3& normal, const glm::dvec3& origin) :
        _normal(glm::normalize(normal)),
        _d(-glm::dot(normal, origin)),
        _coating(ImplicitSurface::NO_COATING)
    {
    }

    Plane::Plane(const glm::dvec4& representation) :
        _normal(representation),
        _d(representation.w),
        _coating(ImplicitSurface::NO_COATING)
    {
    }

    Plane::Plane(double a, double b, double c, double d) :
        _normal(a, b, c),
        _d(d),
        _coating(ImplicitSurface::NO_COATING)
    {
    }

    std::shared_ptr<ImplicitSurface>
        Plane::plane(double a, double b, double c, double d)
    {
        return std::shared_ptr<ImplicitSurface>(new Plane(a, b, c, d));
    }

    std::shared_ptr<ImplicitSurface>
        Plane::plane(const glm::dvec4& representation)
    {
        return std::shared_ptr<ImplicitSurface>(new Plane(representation));
    }

    std::shared_ptr<ImplicitSurface>
        Plane::plane(const glm::dvec3& normal, const glm::dvec3& origin)
    {
        return std::shared_ptr<ImplicitSurface>(new Plane(normal, origin));
    }

    void Plane::transform(const Transform& transform)
    {
        glm::dvec3 pt = glm::dvec3(transform.mat() * glm::dvec4((-_d) * _normal, 1.0));
        _normal = glm::normalize(glm::dvec3(transform.mat() * glm::dvec4(_normal, 0.0)));
        _d = -glm::dot(_normal, pt);
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

    void Plane::raycast(const Ray& ray, RayHitList& reports) const
    {
        double dirDotNorm = glm::dot(_normal, ray.direction);
        if(dirDotNorm != 0.0)
        {
            double t = -(glm::dot(_normal, ray.origin) + _d) / dirDotNorm;
            if(0.0 < t && t < ray.limit)
            {
                glm::dvec3 pt = ray.origin + ray.direction * t;
                reports.add(ray, t, pt, _normal, _coating.get());
            }
        }
    }

    bool Plane::intersects(const Ray& ray, RayHitList& reports) const
    {
        return glm::dot(ray.direction, _normal) != 0.0;
    }

    void Plane::setCoating(const std::shared_ptr<Coating>& coating)
    {
        _coating = coating;
    }

    void Plane::accept(SceneVisitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<SceneNode>> Plane::children() const
    {
        return { _coating };
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

    std::shared_ptr<ImplicitSurface>
        PlaneTexture::plane(double a, double b, double c, double d,
                            const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin)
    {
        return std::shared_ptr<ImplicitSurface>(
                    new PlaneTexture(a, b, c, d, texU, texV, texOrigin));
    }

    std::shared_ptr<ImplicitSurface>
        PlaneTexture::plane(const glm::dvec4& representation,
                            const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin)
    {
        return std::shared_ptr<ImplicitSurface>(
                    new PlaneTexture(representation, texU, texV, texOrigin));
    }

    std::shared_ptr<ImplicitSurface>
        PlaneTexture::plane(const glm::dvec3& normal, const glm::dvec3& origin,
                            const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin)
    {
        return std::shared_ptr<ImplicitSurface>(
                    new PlaneTexture(normal, origin, texU, texV, texOrigin));
    }

    void PlaneTexture::raycast(const Ray& ray, RayHitList& reports) const
    {
        RayHitReport* last = reports.head;
        Plane::raycast(ray, reports);
        RayHitReport* first = reports.head;

        while(first != last)
        {
            RayHitReport& r = *first;
            glm::dvec3 dist = r.position - _texOrigin;
            r.texCoord.s = glm::dot(dist, _texU) / glm::dot(_texU, _texU);
            r.texCoord.t = glm::dot(dist, _texV) / glm::dot(_texV, _texV);

            first = first->_next;
        }
    }

    void PlaneTexture::accept(SceneVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
