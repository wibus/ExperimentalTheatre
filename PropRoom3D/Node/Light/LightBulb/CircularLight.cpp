#include "CircularLight.h"

#include "Node/Visitor.h"
#include "Ray/Raycast.h"
#include "Ray/RayHitList.h"
#include "Ray/RayHitReport.h"
#include "Node/Light/LightCast.h"
#include "Node/Prop/Surface/Disk.h"
#include "Node/Prop/Coating/EmissiveCoating.h"


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
        _radius2(radius*radius),
        _distance(-glm::dot(normal, center)),
        _transformC(_center),
        _transformN(_normal)
    {
        setSurface(Disk::disk(center, normal, radius));
    }

    CircularLight::~CircularLight()
    {

    }

    void CircularLight::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    void CircularLight::fireOn(
            std::vector<LightCast>& lightCasts,
            const glm::dvec3& pos,
            unsigned int count) const
    {
        if(!_isOn) return;

        if(glm::dot(pos - _transformC, _transformN) < 0.0)
            return;

        for(int i=0; i < count; ++i)
        {
            glm::dvec2 disk = _diskRand.gen(_radius);

            glm::dvec3 nonco = glm::abs(_transformN.x) < glm::abs(_transformN.z) ?
                glm::dvec3(1, 0, 0) : glm::dvec3(0, 0, 1);
            glm::dvec3 tangent = glm::normalize(glm::cross(_transformN, nonco));
            glm::dvec3 binorm = glm::normalize(glm::cross(_transformN, tangent));

            glm::dvec3 source = _transformC + tangent*disk.x + binorm*disk.y;
            glm::dvec3 dir = glm::normalize(pos - source);

            glm::dvec4 sample(radiantFlux() / area(), 1.0);
            Raycast ray = Raycast(0.0, sample, source, dir);

            using namespace std::placeholders;
            lightCasts.push_back(LightCast(ray, source, dir,
                std::bind(&CircularLight::diffuseSize, this, _1, _2, _3)));
        }
    }

    double CircularLight::area() const
    {
        return glm::pi<double>() * _radius2;
    }

    double CircularLight::visibility(const Raycast& ray) const
    {
        glm::dvec3 eye = ray.origin - _transformC;
        double eyeNdot = glm::dot(eye, _transformN);

        if(eyeNdot < 0.0)
            return 0.0;

        glm::dvec3 down = glm::normalize(eye - _transformN * eyeNdot) * _radius;
        glm::dvec3 bottom = glm::normalize(down - eye);
        glm::dvec3 top = glm::normalize(-down - eye);

        glm::dvec3 side = glm::cross(down, _transformN);
        glm::dvec3 right = glm::normalize(- side - eye);
        glm::dvec3 left = glm::normalize(side - eye);

        double La = glm::acos(glm::clamp(glm::dot(bottom, top), -1.0, 1.0)) / 2.0;
        double Ta = glm::acos(glm::clamp(glm::dot(right, left), -1.0, 1.0)) / 2.0;

        double theta = glm::sqrt(La * Ta);
        double span = 1 - glm::cos(theta);

        return span;
    }

    void CircularLight::onTransform()
    {
        _transformC = glm::dvec3(_transform * glm::dvec4(_center, 1.0));
        _transformN = glm::transpose(glm::inverse(glm::dmat3(_transform))) * _normal;
    }
}
