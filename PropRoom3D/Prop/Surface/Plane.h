#ifndef PROPROOM3D_PLANE_H
#define PROPROOM3D_PLANE_H

#include "Surface.h"


namespace prop3
{
    class PROP3D_EXPORT Plane : public PhysicalSurface
    {
    protected:
        // From the equation : a*x + b*y + c*z + d = 0
        Plane(double a, double b, double c, double d);

        // From the equation : a*x + b*y + c*z + d = 0
        Plane(const glm::dvec4& representation);

        // Where normal is (a, b, c) and origin is on the plane
        Plane(const glm::dvec3& normal, const glm::dvec3& origin);

    public:
        static std::shared_ptr<Surface>
            plane(double a, double b, double c, double d);

        static std::shared_ptr<Surface>
            plane(const glm::dvec4& representation);

        static std::shared_ptr<Surface>
            plane(const glm::dvec3& normal, const glm::dvec3& origin);


        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;

        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;

        double distance() const;

        glm::dvec3 normal() const;

        glm::dvec4 representation() const;


    private:
        double _d;
        glm::dvec3 _normal;
    };


    class PROP3D_EXPORT PlaneTexture : public Plane
    {
    protected:
        PlaneTexture(double a, double b, double c, double d,
                     const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin);

        PlaneTexture(const glm::dvec4& representation,
                     const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin);

        PlaneTexture(const glm::dvec3& normal, const glm::dvec3& origin,
                     const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin);

    public:
        static std::shared_ptr<Surface>
            plane(double a, double b, double c, double d,
                  const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin);

        static std::shared_ptr<Surface>
            plane(const glm::dvec4& representation,
                  const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin);

        static std::shared_ptr<Surface>
            plane(const glm::dvec3& normal, const glm::dvec3& origin,
                  const glm::dvec3& texU, const glm::dvec3& texV, const glm::dvec3& texOrigin);


        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;

        virtual void transform(const Transform& transform);
        virtual void raycast(const Raycast& ray, RayHitList& reports) const;

        glm::dvec3 texOrigin() const;

        glm::dvec3 texU() const;

        glm::dvec3 texV() const;

    private:
        glm::dvec3 _texOrigin;
        glm::dvec3 _texU, _texV;
    };



    // IMPLEMENTATION //
    inline double Plane::distance() const
    {
        return _d;
    }

    inline glm::dvec3 Plane::normal() const
    {
        return _normal;
    }

    inline glm::dvec4 Plane::representation() const
    {
        return glm::dvec4(_normal, _d);
    }

    inline glm::dvec3 PlaneTexture::texOrigin() const
    {
        return _texOrigin;
    }

    inline glm::dvec3 PlaneTexture::texU() const
    {
        return _texU;
    }

    inline glm::dvec3 PlaneTexture::texV() const
    {
        return _texV;
    }
}

#endif // PROPROOM3D_PLANE_H
