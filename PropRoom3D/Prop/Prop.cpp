#include "Prop.h"

#include <cassert>

#include "Hardware/Hardware.h"
#include "Costume/FlatPaint.h"


namespace prop3
{
    // First assigned ID will be '0'
    PropId Prop::_nextId_ = 0;

    // Inertia
    const double Prop::INFINITE_INERTIA = 0.0;
    const glm::dmat3 Prop::INFINITE_MOMENT_OF_INERTIA(0.0);


    Prop::Prop() :
        _id(_assigneId_()),
        _isVisible(true),
        _costume(new FlatPaint(glm::vec3(1))),
        _material(),
        _bodyType(EBodyType::GRAPHIC),
        _transformMatrix(1.0),
        _invMass(INFINITE_INERTIA),
        _invMomentOfInertia(INFINITE_MOMENT_OF_INERTIA),
        _centroid(0.0),
        _linearVelocity(0.0),
        _linearAcceleration(0.0),
        _linearFirctionCoefficients(0.0),
        _angle(glm::vec3(0.0)),
        _angularVelocity(glm::vec3(0.0)),
        _angularAcceleration(glm::vec3(0.0)),
        _angularFirctionCoefficients(0.0)
    {
    }

    Prop::~Prop()
    {
        if(_material)
        {
            _material->unregisterObserver(*this);
        }
    }

    void Prop::setIsVisible(bool isVisible)
    {
        _isVisible = isVisible;
    }

    void Prop::setVolume(const std::shared_ptr<Volume>& volume)
    {
        _volume = volume;
    }

    const void Prop::setCostume(const std::shared_ptr<Costume>& costume)
    {
        _costume = costume;
    }

    void Prop::setHardware(const std::shared_ptr<Hardware>& material)
    {
        if(_material)
        {
            _material->unregisterObserver(*this);
        }

        _material = material;

        if(_material)
        {
            _material->registerObserver(*this);
        }
    }

    void Prop::notify(HardwareUpdate& msg)
    {
        if(msg.type == HardwareUpdate::EType::DENSITY)
        {
            updateInertia();
        }
    }

    void Prop::setBodyType(const EBodyType& type)
    {
        if(_bodyType != type)
        {
            _bodyType = type;
        }
    }

    void Prop::moveBy(const glm::dvec3& displacement)
    {
        if(displacement != glm::dvec3(0.0))
        {
            _centroid += displacement;
            updateTransformMatrix();
        }
    }

    void Prop::setCentroid(const glm::dvec3& position)
    {
        if(_centroid != position)
        {
            _centroid = position;
            updateTransformMatrix();
        }
    }

    void Prop::setLinearVelocity(const glm::dvec3& velocity)
    {
        _linearVelocity = velocity;
    }

    void Prop::addLinearVelocity(const glm::dvec3& velocity)
    {
        _linearVelocity += velocity;
    }

    void Prop::setLinearAcceleration(const glm::dvec3& acceleration)
    {
        _linearAcceleration = acceleration;
    }

    void Prop::addLinearAcceleration(const glm::dvec3& acceleration)
    {
        _linearAcceleration += acceleration;
    }

    void Prop::setLinearFrictionCoefficients(const glm::dvec3& coeffs)
    {
        _linearFirctionCoefficients = coeffs;
    }

    void Prop::setLinearFrictionCoefficient(int order, double coeff)
    {
        assert(order < 3);
        _linearFirctionCoefficients[order] = coeff;
    }

    void Prop::rotate(const glm::dquat& angle)
    {
        if(angle != glm::dquat(glm::dvec3(0.0)))
        {
            _angle = angle * _angle;
            updateTransformMatrix();
        }
    }

    void Prop::setAngle(const glm::dquat& angle)
    {
        if(_angle != angle)
        {
            _angle = angle;
            updateTransformMatrix();
        }
    }

    void Prop::setAngularVelocity(const glm::dquat& velocity)
    {
        _angularVelocity = velocity;
    }

    void Prop::addAngularVelocity(const glm::dquat& velocity)
    {
        _angularVelocity = velocity * _angularVelocity;
    }

    void Prop::setAngularAcceleration(const glm::dquat& acceleration)
    {
        _angularAcceleration = acceleration;
    }

    void Prop::addAngularAcceleration(const glm::dquat& acceleration)
    {
        _angularAcceleration = acceleration * _angularAcceleration;
    }

    void Prop::setAngularFrictionCoefficients(const glm::dvec3& coeffs)
    {
        _angularFirctionCoefficients = coeffs;
    }

    void Prop::setAngularFrictionCoefficient(int order, double coeff)
    {
        assert(order < 3);
        _angularFirctionCoefficients[order] = coeff;
    }

    void Prop::addLinearForce(const glm::dvec3& force)
    {
        if(_bodyType == EBodyType::DYNAMIC)
            _linearAcceleration += force * _invMass;
    }

    void Prop::addAngularForce(const glm::dvec3& force)
    {
        if(_bodyType == EBodyType::DYNAMIC)
        {
            glm::dvec3 acc = _invRotMomentOfInertia * force;
            double accLen = length(acc);
            if(accLen != 0.0)
                _angularAcceleration = glm::angleAxis(accLen, acc / accLen) * _angularAcceleration;
        }
    }

    void Prop::addForceAt(const glm::dvec3& force, const glm::dvec3& at)
    {
        addLinearForce(force);

        glm::dvec3 radius = at - _centroid;
        addAngularForce(glm::cross(radius, force));
    }

    void Prop::applyLinearImpulse(const glm::dvec3& impulse)
    {
        if(_bodyType == EBodyType::DYNAMIC)
            _linearVelocity += impulse * _invMass;
    }

    void Prop::applyAngularImpulse(const glm::dvec3& impulse)
    {
        if(_bodyType == EBodyType::DYNAMIC)
        {
            glm::dvec3 imp = _invRotMomentOfInertia * impulse;
            double impLen = length(imp);
            if(impLen != 0.0)
                _angularVelocity = glm::angleAxis(impLen, imp / impLen) * _angularVelocity;
        }
    }

    void Prop::applyImpulseAt(const glm::dvec3& impulse, const glm::dvec3& at)
    {
        applyLinearImpulse(impulse);

        glm::dvec3 radius = at - _centroid;
        applyAngularImpulse(glm::cross(radius, impulse));
    }

    void Prop::updateTransformMatrix()
    {
        assert(false /* Not Implemented */);
    }

    void Prop::updateInertia()
    {
        assert(false /* Not Implemented */);
    }
}
