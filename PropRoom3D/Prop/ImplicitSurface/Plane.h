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


        // SceneNode interface
        virtual void accept(SceneVisitor& visitor) override;

        virtual std::vector<std::shared_ptr<SceneNode>> children() const override;


        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, RayHitList& reports) const;
        virtual bool intersects(const Ray& ray, RayHitList& reports) const;

        virtual void setCoating(const std::shared_ptr<Coating>& coating);

        double distance() const;

        glm::dvec3 normal() const;

        std::shared_ptr<Coating> coating() const;


    private:
        double _d;
        glm::dvec3 _normal;
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


        // SceneNode interface
        virtual void accept(SceneVisitor& visitor) override;


        virtual void raycast(const Ray& ray, RayHitList& reports) const;

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

    inline std::shared_ptr<Coating> Plane::coating() const
    {
        return _coating;
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
