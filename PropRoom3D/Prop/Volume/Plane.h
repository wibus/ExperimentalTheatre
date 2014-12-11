#ifndef PROPROOM3D_PLANE_H
#define PROPROOM3D_PLANE_H

#include "Volume.h"


namespace prop3
{
    class PROP3D_EXPORT Plane : public Volume
    {
    public:
        // Where normal is (a, b, c) and point is on the plane
        Plane(const glm::dvec3& normal, const glm::dvec3& point);
        // From the equation : a*x + b*y + c*z + d = 0
        Plane(double a=1, double b=0, double c=0, double d=0);
        virtual ~Plane();

        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;

    private:
        glm::dvec3 _normal;
        double _d;
    };
}

#endif // PROPROOM3D_PLANE_H
