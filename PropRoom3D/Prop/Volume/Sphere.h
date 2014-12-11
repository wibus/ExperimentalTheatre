#ifndef PROPROOM3D_SPHERE_H
#define PROPROOM3D_SPHERE_H

#include "Volume.h"


namespace prop3
{
    class PROP3D_EXPORT Sphere : public Volume
    {
    public:
        Sphere(const glm::dvec3& center = glm::dvec3(0),
                       double radius = 1.0);
        virtual ~Sphere();

        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;

    private:
        double _radius;
        double _radius2;
        glm::dvec3 _center;
    };
}

#endif // PROPROOM3D_SPHERE_H
