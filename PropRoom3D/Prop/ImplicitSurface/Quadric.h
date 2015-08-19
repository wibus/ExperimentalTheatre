#ifndef PROPROOM3D_QUADRIC_H
#define PROPROOM3D_QUADRIC_H

#include "ImplicitSurface.h"


namespace prop3
{
    class PROP3D_EXPORT Quadric : public ImplicitSurface
    {
    public:

        // Ellipsoid : x^2/rx^2 + y^2/ry^2 + z^2/rz^2 = 1
        static std::shared_ptr<ImplicitSurface>
            ellipsoid(double rx, double ry, double rz);

        // Elliptic cone : x^2/rx^2 + y^2/ry^2 - z^2 = 0
        static std::shared_ptr<ImplicitSurface>
            cone(double rx, double ry);

        // Elliptic paraboloid : x^2/rx^2 + y^2/ry^2 - z = 0
        static std::shared_ptr<ImplicitSurface>
            paraboloid(double rx, double ry);

        // Elliptic cylinder : x^2/rx^2 + y^2/ry^2 = 0
        static std::shared_ptr<ImplicitSurface>
            cylinder(double rx, double ry);

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


        // SceneNode interface
        virtual void accept(SceneVisitor& visitor) override;

        virtual std::vector<std::shared_ptr<SceneNode>> children() const override;


        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, RayHitList& reports) const;
        virtual bool intersects(const Ray& ray, RayHitList& reports) const;

        virtual void setCoating(const std::shared_ptr<Coating>& coating);

        glm::dmat4 matrixRepresentation() const;

        std::shared_ptr<Coating> coating() const;



    protected:
        void params(const Ray& ray, double& a, double& b, double& c) const;

    private:
        glm::dmat4 _q;
        std::shared_ptr<Coating> _coating;
    };



    // IMPLEMENTATION //
    inline glm::dmat4 Quadric::matrixRepresentation() const
    {
        return _q;
    }

    inline std::shared_ptr<Coating> Quadric::coating() const
    {
        return _coating;
    }
}

#endif // PROPROOM3D_QUADRIC_H
