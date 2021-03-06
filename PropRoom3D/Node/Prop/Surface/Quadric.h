#ifndef PROPROOM3D_QUADRIC_H
#define PROPROOM3D_QUADRIC_H

#include "Surface.h"


namespace prop3
{
    class PROP3D_EXPORT Quadric : public PhysicalSurface
    {
    protected:
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
        Quadric(double A, double E, double H,
                double B, double C, double D,
                double F, double G, double I,
                double J);

    public:
        static std::shared_ptr<Surface>
            fromMatrix(const glm::dmat4& Q);

        static std::shared_ptr<Surface>
            fromCoeffs(double A, double E, double H,
                       double B, double C, double D,
                       double F, double G, double I,
                       double J);

        // Ellipsoid : x^2/rx^2 + y^2/ry^2 + z^2/rz^2 = 1
        static std::shared_ptr<Surface>
            ellipsoid(double rx, double ry, double rz);

        // Elliptic cone : x^2/rx^2 + y^2/ry^2 - z^2 = 0
        static std::shared_ptr<Surface>
            cone(double rx, double ry);

        // Elliptic paraboloid : x^2/rx^2 + y^2/ry^2 - z = 0
        static std::shared_ptr<Surface>
            paraboloid(double rx, double ry);

        // Elliptic cylinder : x^2/rx^2 + y^2/ry^2 = 0
        static std::shared_ptr<Surface>
            cylinder(double rx, double ry);


        // Node interface
        virtual void accept(Visitor& visitor) override;

        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;

        glm::dmat4 representation() const;

        virtual bool isAffineTransformable() const override {return true;}
        virtual bool isTranslatable() const override {return true;}
        virtual bool isRotatable() const override {return true;}
        virtual bool isScalable() const override {return true;}

    protected:
        virtual void transform(const Transform& transform) override;

        void params(const Raycast& ray, double& a, double& b, double& c) const;

    private:
        glm::dmat4 _q;
    };



    // IMPLEMENTATION //
    inline glm::dmat4 Quadric::representation() const
    {
        return _q;
    }
}

#endif // PROPROOM3D_QUADRIC_H
