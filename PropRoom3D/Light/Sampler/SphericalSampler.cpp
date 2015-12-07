#include "SphericalSampler.h"

#include <GLM/gtc/constants.hpp>

#include "Node/Visitor.h"
#include "Ray/Raycast.h"


namespace prop3
{
    SphericalSampler::SphericalSampler(
            const glm::dvec3& center,
            double radius) :
        _center(center),
        _radius(radius)
    {

    }

    SphericalSampler::~SphericalSampler()
    {

    }

    void SphericalSampler::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    double SphericalSampler::area() const
    {
        return 4.0 * glm::pi<double>() * _radius * _radius;
    }

    double SphericalSampler::hitProbability(const Raycast& ray) const
    {
        glm::dvec3 dist = ray.origin - _center;

        double length = glm::max(glm::length(dist), _radius);
        double span = 1.0 - glm::cos(glm::asin(_radius / length));

        return span;
    }

    void SphericalSampler::setCenter(const glm::dvec3& center)
    {
        _center = center;

        stampCurrentUpdate();
    }

    void SphericalSampler::setRadius(double radius)
    {
        _radius = radius;

        stampCurrentUpdate();
    }
}
