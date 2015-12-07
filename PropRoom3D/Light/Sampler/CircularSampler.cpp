#include "CircularSampler.h"

#include <GLM/gtc/constants.hpp>

#include "Node/Visitor.h"
#include "Ray/Raycast.h"


namespace prop3
{
    CircularSampler::CircularSampler(
            bool isTowSided,
            const glm::dvec3& center,
            const glm::dvec3& normal,
            double radius) :
        _isTwoSided(isTowSided),
        _center(center),
        _normal(normal),
        _radius(radius)
    {

    }

    CircularSampler::~CircularSampler()
    {

    }

    void CircularSampler::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    double CircularSampler::area() const
    {
        return glm::pi<double>() * _radius * _radius;
    }

    double CircularSampler::hitProbability(const Raycast& ray) const
    {
        glm::dvec3 eye = ray.origin - _center;
        double eyeNdot = glm::dot(eye, _normal);

        if(!_isTwoSided && eyeNdot < 0.0)
            return 0.0;

        glm::dvec3 down = glm::normalize(eye - _normal * eyeNdot) * _radius;
        glm::dvec3 bottom = glm::normalize(down - eye);
        glm::dvec3 top = glm::normalize(-down - eye);

        glm::dvec3 side = glm::cross(down, _normal);
        glm::dvec3 right = glm::normalize(- side - eye);
        glm::dvec3 left = glm::normalize(side - eye);

        double La = glm::acos(glm::dot(bottom, top)) / 2.0;
        double Ta = glm::acos(glm::dot(right, left)) / 2.0;

        double theta = glm::sqrt(La * Ta);
        double span = 1 - glm::cos(theta);

        return span;
    }

    void CircularSampler::setIsTwoSided(bool isTwoSided)
    {
        _isTwoSided = isTwoSided;

        stampCurrentUpdate();
    }

    void CircularSampler::setCenter(const glm::dvec3& center)
    {
        _center = center;

        stampCurrentUpdate();
    }

    void CircularSampler::setNormal(const glm::dvec3& normal)
    {
        _normal = glm::normalize(normal);

        stampCurrentUpdate();
    }

    void CircularSampler::setRadius(double radius)
    {
        _radius = radius;

        stampCurrentUpdate();
    }
}
