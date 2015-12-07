#ifndef PROPROOM3D_SPHERICALSAMPLER_H
#define PROPROOM3D_SPHERICALSAMPLER_H

#include <GLM/glm.hpp>

#include "Sampler.h"


namespace prop3
{
    class PROP3D_EXPORT SphericalSampler : public Sampler
    {
    public:
        SphericalSampler(const glm::dvec3& center,
                         double radius);

        virtual ~SphericalSampler();


        virtual void accept(Visitor& visito) override;


        virtual double area() const override;

        virtual double hitProbability(const Raycast& ray) const override;


        void setCenter(const glm::dvec3& center);
        glm::dvec3 center() const;

        void setRadius(double radius);
        double radius() const;


    private:
        glm::dvec3 _center;
        double _radius;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 SphericalSampler::center() const
    {
        return _center;
    }

    inline double SphericalSampler::radius() const
    {
        return _radius;
    }
}

#endif // PROPROOM3D_CIRCULARSAMPLER_H
