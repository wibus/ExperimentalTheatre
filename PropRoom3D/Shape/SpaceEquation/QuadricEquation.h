#ifndef PROPROOM3D_QUADRIC_EQUATION_H
#define PROPROOM3D_QUADRIC_EQUATION_H

#include "SpaceEquation.h"


namespace prop3
{
    class PROP3D_EXPORT PlaneEquation
    {
    public:
        // Where normal is (a, b, c) and point is on the plane
        PlaneEquation(const glm::dvec3& normal, const glm::dvec3& point);
        // From the equation : a*x + b*y + c*z + d = 0
        PlaneEquation(double a=1, double b=0, double c=0, double d=0);
        virtual ~PlaneEquation();

        virtual bool isIn(const glm::dvec3& point) const;
        virtual double computeSignedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;

    private:
        glm::dvec3 _normal;
        double _d;
    };

    class PROP3D_EXPORT SphereEquation
    {
    public:
        SphereEquation(const glm::dvec3& center = glm::dvec3(0),
                       double radius = 1.0);
        virtual ~SphereEquation();

        virtual bool isIn(const glm::dvec3& point) const;
        virtual double computeSignedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;

    private:
        double _radius;
        double _radius2;
        glm::dvec3 _center;
    };
}

#endif // PROPROOM3D_QUADRIC_EQUATION_H
