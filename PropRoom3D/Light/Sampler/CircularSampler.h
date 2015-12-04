#ifndef PROPROOM3D_CIRCULARSAMPLER_H
#define PROPROOM3D_CIRCULARSAMPLER_H

#include <GLM/glm.hpp>

#include "Sampler.h"


namespace prop3
{
    class PROP3D_EXPORT CircularSampler : public Sampler
    {
    public:
        CircularSampler();

        virtual ~CircularSampler();


        virtual void accept(Visitor& visito) override;


        virtual double area() const override;

        virtual double hitProbability(const Raycast& ray) const override;


        void setIsTwoSided(bool isTwoSided);
        bool isTowSided() const;

        void setCenter(const glm::dvec3& center);
        glm::dvec3 center() const;

        void setNormal(const glm::dvec3& normal);
        glm::dvec3 normal() const;

        void setRadius(double radius);
        double radius() const;


    private:
        bool _isTwoSided;
        glm::dvec3 _center;
        glm::dvec3 _normal;
        double _radius;
    };



    // IMPLEMENTATION //
    inline bool CircularSampler::isTowSided() const
    {
        return _isTwoSided;
    }

    inline glm::dvec3 CircularSampler::center() const
    {
        return _center;
    }

    inline glm::dvec3 CircularSampler::normal() const
    {
        return _normal;
    }

    inline double CircularSampler::radius() const
    {
        return _radius;
    }
}

#endif // PROPROOM3D_CIRCULARSAMPLER_H
