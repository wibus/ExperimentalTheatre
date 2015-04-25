#ifndef PROPROOM3D_QUADRIC_H
#define PROPROOM3D_QUADRIC_H

#include "Volume.h"


namespace prop3
{
    class PROP3D_EXPORT Quadric : public Volume
    {
    public:
        // Constructing a quadric from the equation :
        // Ax^2 + 2Bxy + 2Cxz + 2Dx  +  Ey^2 + 2Fyz + 2Gy + Hz^2 + Iz  +  J = 0
        // Disposing the coefficients that way :
        //      [A B C D]
        //  Q = [B E F G]
        //      [C F H I]
        //      [D G I J]
        Quadric(const glm::dmat4& Q);

        // Constructing a quadric from the equation :
        // Ax^2 + 2Bxy + 2Cxz + 2Dx  +  Ey^2 + 2Fyz + 2Gy + Hz^2 + Iz  +  J = 0
        Quadric(double A, double B, double C,
                double D, double E, double F,
                double G, double H, double I,
                double J);

        virtual ~Quadric();

        virtual std::shared_ptr<Volume> clone() const;
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;

    private:
        glm::dmat4 _q;
    };
}

#endif // PROPROOM3D_QUADRIC_H
