#ifndef PROPROOM3D_SPHERE_H
#define PROPROOM3D_SPHERE_H

#include "Surface.h"


namespace prop3
{
    class PROP3D_EXPORT Sphere : public Surface
    {
        Sphere(const glm::dvec3& center, double radius);

    public:
        static std::shared_ptr<Surface>
            sphere(const glm::dvec3& center, double radius);

        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;

        virtual std::vector<std::shared_ptr<StageSetNode>> children() const override;


        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;

        virtual void setCoating(const std::shared_ptr<Coating>& coating);

        double radius() const;

        glm::dvec3 center() const;

        std::shared_ptr<Coating> coating() const;


protected:
        void params(const Raycast& ray, double& a, double& b, double& c) const;

    private:
        double _radius;
        double _radius2;
        glm::dvec3 _center;
        std::shared_ptr<Coating> _coating;
    };



    // IMPLEMENTATION //
    inline double Sphere::radius() const
    {
        return _radius;
    }

    inline glm::dvec3 Sphere::center() const
    {
        return _center;
    }

    inline std::shared_ptr<Coating> Sphere::coating() const
    {
        return _coating;
    }
}

#endif // PROPROOM3D_SPHERE_H
