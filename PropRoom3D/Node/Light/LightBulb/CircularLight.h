#ifndef PROPROOM3D_CIRCULARLIGHT_H
#define PROPROOM3D_CIRCULARLIGHT_H

#include <GLM/glm.hpp>

#include <CellarWorkbench/Misc/Distribution.h>

#include "LightBulb.h"


namespace prop3
{
    class PROP3D_EXPORT CircularLight : public LightBulb
    {
    public:
        CircularLight(const std::string& name,
                      const glm::dvec3& center,
                      const glm::dvec3& normal,
                      double radius);

        virtual ~CircularLight();


        virtual void accept(Visitor& visitor) override;


        // Light rays
        virtual void fireOn(
                std::vector<LightCast>& lightCasts,
                const glm::dvec3& pos,
                unsigned int count) const override;


        // Properties
        virtual double area() const override;

        virtual double visibility(const Raycast& ray) const override;


        glm::dvec3 center() const;

        glm::dvec3 normal() const;

        double radius() const;


    protected:
        virtual void onTransform() override;


    private:
        glm::dvec3 _center;
        glm::dvec3 _normal;
        double _radius;
        double _radius2;

        double _distance;
        glm::dvec3 _transformC;
        glm::dvec3 _transformN;

        cellar::DiskRand _diskRand;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 CircularLight::center() const
    {
        return _center;
    }

    inline glm::dvec3 CircularLight::normal() const
    {
        return _normal;
    }

    inline double CircularLight::radius() const
    {
        return _radius;
    }
}

#endif // PROPROOM3D_CIRCULARLIGHT_H
