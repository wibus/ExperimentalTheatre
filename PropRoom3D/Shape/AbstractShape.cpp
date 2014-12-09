#include "AbstractShape.h"
#include "Hardware/Hardware.h"
#include "Costume/FlatPaint.h"

#include <cassert>


namespace prop3
{
    // First assigned ID will be '0'
    PropId AbstractShape::_nextId_ = 0;

    // Inertia
    const double AbstractShape::INFINITE_INERTIA = 0.0;
    const glm::dmat3 AbstractShape::INFINITE_MOMENT_OF_INERTIA(0.0);


    AbstractShape::AbstractShape(EPropType propType) :
        _id(_assigneId_()),
        _propType(propType),
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

    AbstractShape::~AbstractShape()
    {
        if(_material)
        {
            _material->unregisterObserver(*this);
        }
    }

    const std::shared_ptr<AbstractCostume>& AbstractShape::costume() const
    {
        return _costume;
    }

    const void AbstractShape::setCostume(const std::shared_ptr<AbstractCostume>& costume)
    {
        _costume = costume;
    }

    const std::shared_ptr<Hardware>& AbstractShape::hardware() const
    {
        return _material;
    }

    void AbstractShape::setHardware(const std::shared_ptr<Hardware>& material)
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

    void AbstractShape::notify(HardwareUpdate& msg)
    {
        if(msg.type == HardwareUpdate::EType::DENSITY)
        {
            updateInertia();
        }
    }

    EBodyType AbstractShape::bodyType() const
    {
        return _bodyType;
    }

    void AbstractShape::setBodyType(const EBodyType& type)
    {
        if(_bodyType != type)
        {
            _bodyType = type;
        }
    }

    double AbstractShape::mass() const
    {
        return _bodyType ==  EBodyType::DYNAMIC && _invMass ?
            1.0 / _invMass :
            INFINITE_INERTIA;
    }

    double AbstractShape::inverseMass() const
    {
        return _bodyType ==  EBodyType::DYNAMIC ?
            _invMass :
            INFINITE_INERTIA;
    }

    glm::dmat3 AbstractShape::momentOfInertia() const
    {
        return _bodyType ==  EBodyType::DYNAMIC && _invMomentOfInertia[0][0] != 0.0?
            glm::inverse(_invMomentOfInertia) :
            INFINITE_MOMENT_OF_INERTIA;
    }

    glm::dmat3 AbstractShape::inverseMomentOfInertia() const
    {
        return _bodyType ==  EBodyType::DYNAMIC ?
            _invMomentOfInertia :
            INFINITE_MOMENT_OF_INERTIA;
    }

    glm::dvec3 AbstractShape::centroid() const
    {
        return _centroid;
    }

    void AbstractShape::moveBy(const glm::dvec3& displacement)
    {
        if(displacement != glm::dvec3(0.0))
        {
            _centroid += displacement;
            updateTransformMatrix();
        }
    }

    void AbstractShape::setCentroid(const glm::dvec3& position)
    {
        if(_centroid != position)
        {
            _centroid = position;
            updateTransformMatrix();
        }
    }

    glm::dvec3 AbstractShape::linearVelocity() const
    {
        return _linearVelocity;
    }

    void AbstractShape::setLinearVelocity(const glm::dvec3& velocity)
    {
        _linearVelocity = velocity;
    }

    void AbstractShape::addLinearVelocity(const glm::dvec3& velocity)
    {
        _linearVelocity += velocity;
    }

    glm::dvec3 AbstractShape::linearAcceleration() const
    {
        return _linearAcceleration;
    }

    void AbstractShape::setLinearAcceleration(const glm::dvec3& acceleration)
    {
        _linearAcceleration = acceleration;
    }

    void AbstractShape::addLinearAcceleration(const glm::dvec3& acceleration)
    {
        _linearAcceleration += acceleration;
    }

    glm::dvec3 AbstractShape::linearFrictionCoefficients() const
    {
        return _linearFirctionCoefficients;
    }

    void AbstractShape::setLinearFrictionCoefficients(const glm::dvec3& coeffs)
    {
        _linearFirctionCoefficients = coeffs;
    }

    void AbstractShape::setLinearFrictionCoefficient(int order, double coeff)
    {
        assert(order < 3);
        _linearFirctionCoefficients[order] = coeff;
    }

    glm::dquat AbstractShape::angle() const
    {
        return _angle;
    }

    void AbstractShape::rotate(const glm::dquat& angle)
    {
        if(angle != glm::dquat(glm::dvec3(0.0)))
        {
            _angle = angle * _angle;
            updateTransformMatrix();
        }
    }

    void AbstractShape::setAngle(const glm::dquat& angle)
    {
        if(_angle != angle)
        {
            _angle = angle;
            updateTransformMatrix();
        }
    }

    glm::dquat AbstractShape::angularVelocity() const
    {
        return _angularVelocity;
    }

    void AbstractShape::setAngularVelocity(const glm::dquat& velocity)
    {
        _angularVelocity = velocity;
    }

    void AbstractShape::addAngularVelocity(const glm::dquat& velocity)
    {
        _angularVelocity = velocity * _angularVelocity;
    }

    glm::dquat AbstractShape::angularAcceleration() const
    {
        return _angularAcceleration;
    }

    void AbstractShape::setAngularAcceleration(const glm::dquat& acceleration)
    {
        _angularAcceleration = acceleration;
    }

    void AbstractShape::addAngularAcceleration(const glm::dquat& acceleration)
    {
        _angularAcceleration = acceleration * _angularAcceleration;
    }

    glm::dvec3 AbstractShape::angularFrictionCoefficients() const
    {
        return _angularFirctionCoefficients;
    }

    void AbstractShape::setAngularFrictionCoefficients(const glm::dvec3& coeffs)
    {
        _angularFirctionCoefficients = coeffs;
    }

    void AbstractShape::setAngularFrictionCoefficient(int order, double coeff)
    {
        assert(order < 3);
        _angularFirctionCoefficients[order] = coeff;
    }

    void AbstractShape::addLinearForce(const glm::dvec3& force)
    {
        if(_bodyType == EBodyType::DYNAMIC)
            _linearAcceleration += force * _invMass;
    }

    void AbstractShape::addAngularForce(const glm::dvec3& force)
    {
        if(_bodyType == EBodyType::DYNAMIC)
        {
            glm::dvec3 acc = _invRotMomentOfInertia * force;
            double accLen = length(acc);
            if(accLen != 0.0)
                _angularAcceleration = glm::angleAxis(accLen, acc / accLen) * _angularAcceleration;
        }
    }

    void AbstractShape::addForceAt(const glm::dvec3& force, const glm::dvec3& at)
    {
        addLinearForce(force);

        glm::dvec3 radius = at - _centroid;
        addAngularForce(glm::cross(radius, force));
    }

    void AbstractShape::applyLinearImpulse(const glm::dvec3& impulse)
    {
        if(_bodyType == EBodyType::DYNAMIC)
            _linearVelocity += impulse * _invMass;
    }

    void AbstractShape::applyAngularImpulse(const glm::dvec3& impulse)
    {
        if(_bodyType == EBodyType::DYNAMIC)
        {
            glm::dvec3 imp = _invRotMomentOfInertia * impulse;
            double impLen = length(imp);
            if(impLen != 0.0)
                _angularVelocity = glm::angleAxis(impLen, imp / impLen) * _angularVelocity;
        }
    }

    void AbstractShape::applyImpulseAt(const glm::dvec3& impulse, const glm::dvec3& at)
    {
        applyLinearImpulse(impulse);

        glm::dvec3 radius = at - _centroid;
        applyAngularImpulse(glm::cross(radius, impulse));
    }
}
