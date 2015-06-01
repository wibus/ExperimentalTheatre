#ifndef PROPROOM3D_SPHERE_H
#define PROPROOM3D_SPHERE_H

#include "ImplicitSurface.h"


namespace prop3
{
    class PROP3D_EXPORT Sphere : public ImplicitSurface
    {
        Sphere(const glm::dvec3& center, double radius);

    public:
        static std::shared_ptr<ImplicitSurface>
            sphere(const glm::dvec3& center, double radius);

        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RayHitReport>& reports) const;
        virtual bool intersects(const Ray& ray);

        virtual void setCoating(const std::shared_ptr<Coating>& coating);

protected:
        void params(const Ray& ray, double& a, double& b, double& c) const;

    private:
        double _radius;
        double _radius2;
        glm::dvec3 _center;
        std::shared_ptr<Coating> _coating;
    };
}

#endif // PROPROOM3D_SPHERE_H
