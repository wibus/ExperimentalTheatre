#include "LightBulb.h"

#include <GLM/gtc/matrix_transform.hpp>

#include "Ray/Raycast.h"
#include "Prop/Surface/Surface.h"
#include "Prop/Coating/EmissiveCoating.h"


namespace prop3
{
    LightBulb::LightBulb(const std::string& name) :
        HandleNode(name),
        _isOn(true),
        _radiantFlux(0.0),
        _coating(new EmissiveCoating(glm::dvec3(0.0)))
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
        _transform = mat * _transform;
        _invTransform = _invTransform * glm::inverse(mat);

        onTransform();

        stampCurrentUpdate();
    }

    void LightBulb::translate(const glm::dvec3& dis)
    {
        transform(glm::translate(glm::dmat4(), dis));
    }

    void LightBulb::rotate(double angle, const glm::dvec3& axis)
    {
        transform(glm::rotate(glm::dmat4(), angle, axis));
    }

    void LightBulb::scale(double coeff)
    {
        transform(glm::scale(glm::dmat4(), glm::dvec3(coeff)));
    }

    void LightBulb::setIsOn(bool isOn)
    {
        _isOn = isOn;

        if(_isOn)
        {
            _coating->setEmittedRadiance(
                _radiantFlux / area());
        }
        else
        {
            _coating->setEmittedRadiance(
                glm::dvec3(0.0));
        }

        stampCurrentUpdate();
    }

    void LightBulb::setRadiantFlux(const glm::dvec3& radiantFlux)
    {
        _radiantFlux = radiantFlux;

        if(_isOn)
        {
            _coating->setEmittedRadiance(
                _radiantFlux / area());
        }

        stampCurrentUpdate();
    }

    void LightBulb::setOuterMaterial(const std::shared_ptr<Material>& mat)
    {
        _outerMat = mat;

        stampCurrentUpdate();
    }

    void LightBulb::setTransform(const glm::dmat4& transform)
    {
        _transform = transform;
        _invTransform = glm::inverse(_transform);

        onTransform();

        stampCurrentUpdate();
    }
}
