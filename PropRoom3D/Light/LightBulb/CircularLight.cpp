#include "CircularLight.h"

#include <GLM/gtc/constants.hpp>

#include "Node/Visitor.h"
#include "Ray/Raycast.h"
#include "Ray/RayHitList.h"
#include "Ray/RayHitReport.h"
#include "Prop/Coating/EmissiveCoating.h"


namespace prop3
{
    CircularLight::CircularLight(
            const std::string& name,
            const glm::dvec3& center,
            const glm::dvec3& normal,
            double radius) :
        LightBulb(name),
        _center(center),
        _normal(normal),
        _radius(radius),
        _distance(-glm::dot(normal, center)),
        _transformN(_normal)
    {

    }

    CircularLight::~CircularLight()
    {

    }

    void CircularLight::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    void CircularLight::raycast(const Raycast& ray, RayHitList& reports) const
    {
        glm::dvec3 orig = glm::dvec3(_invTransform * glm::dvec4(ray.origin, 1.0));
        glm::dvec3 dir = glm::dvec3(_invTransform * glm::dvec4(ray.direction, 0.0));


        double dirDotNorm = glm::dot(_normal, dir);
        if(dirDotNorm != 0.0)
        {
            double t = -(glm::dot(_normal, orig) + _distance) / dirDotNorm;
            if(0.0 < t && t < ray.limit)
            {
                if(glm::distance(orig + dir * t, _center) < _radius)
                {
                    glm::dvec3 pt = ray.origin + ray.direction * t;
                    reports.add(t, pt, _transformN,
                                RayHitReport::NO_TEXCOORD,
                                _coating.get(),
                                nullptr,
                                _outerMat.get());
                }
            }
        }
    }

    bool CircularLight::intersects(const Raycast& ray, RayHitList& reports) const
    {
        glm::dvec3 orig = glm::dvec3(_invTransform * glm::dvec4(ray.origin, 1.0));
        glm::dvec3 dir = glm::dvec3(_invTransform * glm::dvec4(ray.direction, 0.0));


        double dirDotNorm = glm::dot(_normal, dir);
        if(dirDotNorm != 0.0)
        {
            double t = -(glm::dot(_normal, orig) + _distance) / dirDotNorm;
            if(0.0 < t && t < ray.limit)
            {
                if(glm::distance(orig + dir * t, _center) < _radius)
                    return true;
            }
        }

        return false;
    }

    void CircularLight::fireOn(
            std::vector<LightCast>& lightCasts,
            const glm::dvec3& pos,
            unsigned int count) const
    {
        // TODO
    }

    double CircularLight::area() const
    {
        return glm::pi<double>() * _radius * _radius;
    }

    double CircularLight::visibility(const Raycast& ray) const
    {
        glm::dvec3 eye = ray.origin - _center;
        double eyeNdot = glm::dot(eye, _normal);

        if(eyeNdot < 0.0)
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

    void CircularLight::onTransform()
    {
        _transformN = glm::transpose(glm::inverse(glm::dmat3(_transform))) * _normal;
    }
}
