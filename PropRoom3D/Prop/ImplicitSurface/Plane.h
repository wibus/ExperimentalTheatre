#ifndef PROPROOM3D_PLANE_H
#define PROPROOM3D_PLANE_H

#include "ImplicitSurface.h"


namespace prop3
{
    class PROP3D_EXPORT Plane : public ImplicitSurface
    {
    public:
        // Where normal is (a, b, c) and point is on the plane
        Plane(const glm::dvec3& normal, const glm::dvec3& point);

        // From the equation : a*x + b*y + c*z + d = 0
        Plane(double a, double b, double c, double d);
        virtual ~Plane();

        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RayHitReport>& reports) const;
        virtual bool intersects(const Ray& ray);

        virtual void setCoating(const std::shared_ptr<Coating>& coating);

    private:
        glm::dvec3 _normal;
        double _d;
        std::shared_ptr<Coating> _coating;
    };


    class PROP3D_EXPORT PlaneTexture : public Plane
    {
    public:
        // Where normal is (a, b, c) and point is on the plane
        PlaneTexture(const glm::dvec3& normal, const glm::dvec3& point,
                     const glm::dvec3& u, const glm::dvec3& v, const glm::dvec3& origin);

        // From the equation : a*x + b*y + c*z + d = 0
        PlaneTexture(double a, double b, double c, double d,
                     const glm::dvec3& u, const glm::dvec3& v, const glm::dvec3& origin);

        virtual void raycast(const Ray& ray, std::vector<RayHitReport>& reports) const;

    private:
        glm::dvec3 _origin;
        glm::dvec3 _u, _v;
    };
}

#endif // PROPROOM3D_PLANE_H
