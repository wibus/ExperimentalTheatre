#ifndef PROPROOM3D_PLANE_H
#define PROPROOM3D_PLANE_H

#include "ImplicitSurface.h"


namespace prop3
{
    class PROP3D_EXPORT Plane : public ImplicitSurface
    {
    protected:
        // From the equation : a*x + b*y + c*z + d = 0
        Plane(double a, double b, double c, double d);

        // Where normal is (a, b, c) and origin is on the plane
        Plane(const glm::dvec3& normal, const glm::dvec3& origin);

    public:
        static std::shared_ptr<ImplicitSurface>
            plane(double a, double b, double c, double d);

        static std::shared_ptr<ImplicitSurface>
            plane(const glm::dvec3& normal, const glm::dvec3& origin);

        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, RayHitList& reports) const;
        virtual bool intersects(const Ray& ray, RayHitList& reports) const;

        virtual void setCoating(const std::shared_ptr<Coating>& coating);

    private:
        glm::dvec3 _normal;
        double _d;
        std::shared_ptr<Coating> _coating;
    };


    class PROP3D_EXPORT PlaneTexture : public Plane
    {
    protected:
        PlaneTexture(double a, double b, double c, double d,
                     const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin);

        PlaneTexture(const glm::dvec3& normal, const glm::dvec3& origin,
                     const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin);

    public:
        static std::shared_ptr<ImplicitSurface>
            plane(double a, double b, double c, double d,
                  const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin);

        static std::shared_ptr<ImplicitSurface>
            plane(const glm::dvec3& normal, const glm::dvec3& origin,
                  const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin);

        virtual void raycast(const Ray& ray, RayHitList& reports) const;

    private:
        glm::dvec3 _texOrigin;
        glm::dvec3 _texU, _texV;
    };
}

#endif // PROPROOM3D_PLANE_H
