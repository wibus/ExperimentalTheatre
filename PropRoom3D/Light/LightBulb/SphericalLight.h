#ifndef PROPROOM3D_SPHERICALLIGHT_H
#define PROPROOM3D_SPHERICALLIGHT_H

#include <GLM/glm.hpp>

#include "LightBulb.h"


namespace prop3
{
    class PROP3D_EXPORT SphericalLight : public LightBulb
    {
    public:
        SphericalLight(const std::string& name,
                       const glm::dvec3& center,
                       double radius);

        virtual ~SphericalLight();


        virtual void accept(Visitor& visitor) override;


        // Light rays
        virtual void raycast(const Raycast& ray, RayHitList& reports) const override;

        virtual bool intersects(const Raycast& ray, RayHitList& reports) const override;

        virtual std::vector<Raycast> fireRays(unsigned int count) const override;

        virtual std::vector<Raycast> fireOn(const glm::dvec3& pos, unsigned int count) const override;


        glm::dvec3 center() const;

        double radius() const;


    protected:
        virtual double area() const override;
        virtual double visibility(const Raycast& ray) const override;
        virtual glm::dvec3 genPoint() const override;

        virtual void onTransform() override;


    private:
        glm::dvec3 _center;
        double _radius;

        glm::dvec3 _transformC;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 SphericalLight::center() const
    {
        return _center;
    }

    inline double SphericalLight::radius() const
    {
        return _radius;
    }
}

#endif // PROPROOM3D_SPHERICALLIGHT_H
