#include "LightBulb.h"

#include <GLM/gtc/matrix_transform.hpp>

#include "Ray/Raycast.h"
#include "Node/Light/LightCast.h"
#include "Node/Prop/Surface/Surface.h"
#include "Node/Prop/Coating/EmissiveCoating.h"


namespace prop3
{
    LightBulb::LightBulb(const std::string& name) :
        HandleNode(name),
        _isOn(true),
        _radiantFlux(0.0),
        _coating(new EmissiveCoating(*this))
    {
    }

    LightBulb::~LightBulb()
    {

    }

    void LightBulb::clear()
    {

    }

    void LightBulb::transform(const glm::dmat4& mat)
    {
        Surface::transform(_surface, mat);
        commitTransform(mat);
    }

    void LightBulb::translate(const glm::dvec3& dis)
    {
        Surface::translate(_surface, dis);
        commitTransform(glm::translate(glm::dmat4(), dis));
    }

    void LightBulb::rotate(double angle, const glm::dvec3& axis)
    {
        Surface::rotate(_surface, angle, axis);
        commitTransform(glm::rotate(glm::dmat4(), angle, axis));
    }

    void LightBulb::scale(double coeff)
    {
        Surface::scale(_surface, coeff);
        commitTransform(glm::scale(glm::dmat4(), glm::dvec3(coeff)));
    }

    void LightBulb::setIsOn(bool isOn)
    {
        _isOn = isOn;

        stampCurrentUpdate();
    }

    void LightBulb::setRadiantFlux(const glm::dvec3& radiantFlux)
    {
        _radiantFlux = radiantFlux;

        stampCurrentUpdate();
    }

    void LightBulb::setSurface(const std::shared_ptr<Surface>& surf)
    {
        _surface = surf;
        _surface->setCoating(_coating);

        stampCurrentUpdate();
    }

    double LightBulb::diffuseSize(
            const LightCast& lightCast,
            const Raycast& eyeRay,
            double roughness) const
    {
        double entropy = Raycast::totalEntropy(eyeRay, lightCast.raycast, roughness);
        double diffDist = Raycast::totalDiffuseDist(eyeRay, lightCast.raycast, roughness);
        glm::dvec3 origin = lightCast.emittingPosition + lightCast.emittingDirection * diffDist;
        return visibility(Raycast(entropy, glm::dvec4(), origin, -lightCast.emittingDirection));
    }

    void LightBulb::commitTransform(const glm::dmat4& mat)
    {
        _transform = mat * _transform;
        _invTransform = _invTransform * glm::inverse(mat);

        onTransform();
        stampCurrentUpdate();
    }
}
