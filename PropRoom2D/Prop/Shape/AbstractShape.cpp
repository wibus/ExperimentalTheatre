#include "AbstractShape.h"

#include <cassert>

#include "Segment2D.h"
#include "../Hardware/Hardware.h"


namespace prop2
{

    const double AbstractShape::INFINITE_INERTIA = 0;

    AbstractShape::AbstractShape(EPropType propType) :
        AbstractProp(propType),
        _material(),
        _bodyType(EBodyType::GRAPHIC),
        _tranformMatrix(1.0),
        _inverseMass(INFINITE_INERTIA),
        _inverseMomentOfInertia(INFINITE_INERTIA),
        _centroid(0.0, 0.0),
        _linearVelocity(0.0, 0.0),
        _linearAcceleration(0.0, 0.0),
        _linearFirctionCoefficients(0, 0, 0),
        _angle(0.0),
        _angularVelocity(0.0),
        _angularAcceleration(0.0),
        _angularFirctionCoefficients(0, 0, 0)
    {
    }

    AbstractShape::~AbstractShape()
    {
        if(_material)
        {
            _material->unregisterObserver(*this);
        }
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

    const glm::dmat3& AbstractShape::transformMatrix() const
    {
        return _tranformMatrix;
    }

    double AbstractShape::mass() const
    {
        return _bodyType ==  EBodyType::DYNAMIC && _inverseMass ?
            1/_inverseMass :
            INFINITE_INERTIA;
    }

    double AbstractShape::inverseMass() const
    {
        return _bodyType ==  EBodyType::DYNAMIC ?
            _inverseMass :
            INFINITE_INERTIA;
    }

    double AbstractShape::momentOfInertia() const
    {
        return _bodyType ==  EBodyType::DYNAMIC && _inverseMomentOfInertia ?
            1/_inverseMomentOfInertia :
            INFINITE_INERTIA;
    }

    double AbstractShape::inverseMomentOfInertia() const
    {
        return _bodyType ==  EBodyType::DYNAMIC ?
            _inverseMomentOfInertia :
            INFINITE_INERTIA;
    }

    glm::dvec2 AbstractShape::centroid() const
    {
        return _centroid;
    }

    void AbstractShape::moveBy(const glm::dvec2& displacement)
    {
        if(displacement != glm::dvec2(0))
        {
            _centroid += displacement;
            updateTransformMatrix();
        }
    }

    void AbstractShape::setCentroid(const glm::dvec2& position)
    {
        if(_centroid != position)
        {
            _centroid = position;
            updateTransformMatrix();
        }
    }

    glm::dvec2 AbstractShape::linearVelocity() const
    {
        return _linearVelocity;
    }

    void AbstractShape::setLinearVelocity(const glm::dvec2& velocity)
    {
        _linearVelocity = velocity;
    }

    void AbstractShape::addLinearVelocity(const glm::dvec2& velocity)
    {
        _linearVelocity += velocity;
    }

    glm::dvec2 AbstractShape::linearAcceleration() const
    {
        return _linearAcceleration;
    }

    void AbstractShape::setLinearAcceleration(const glm::dvec2& acceleration)
    {
        _linearAcceleration = acceleration;
    }

    void AbstractShape::addLinearAcceleration(const glm::dvec2& acceleration)
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

    double AbstractShape::angle() const
    {
        return _angle;
    }

    void AbstractShape::rotate(const double& angle)
    {
        if(angle)
        {
            _angle += angle;
            updateTransformMatrix();
        }
    }

    void AbstractShape::setAngle(const double& angle)
    {
        if(_angle != angle)
        {
            _angle = angle;
            updateTransformMatrix();
        }
    }

    double AbstractShape::angularVelocity() const
    {
        return _angularVelocity;
    }

    void AbstractShape::setAngularVelocity(const double& velocity)
    {
        _angularVelocity = velocity;
    }

    void AbstractShape::addAngularVelocity(const double& velocity)
    {
        _angularVelocity += velocity;
    }

    double AbstractShape::angularAcceleration() const
    {
        return _angularAcceleration;
    }

    void AbstractShape::setAngularAcceleration(const double& acceleration)
    {
        _angularAcceleration = acceleration;
    }

    void AbstractShape::addAngularAcceleration(const double& acceleration)
    {
        _angularAcceleration += acceleration;
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

    void AbstractShape::addLinearForce(const glm::dvec2& force)
    {
        if(_bodyType == EBodyType::DYNAMIC)
            _linearAcceleration += force * _inverseMass;
    }

    void AbstractShape::addAngularForce(const double& force)
    {
        if(_bodyType == EBodyType::DYNAMIC)
            _angularAcceleration += force * _inverseMomentOfInertia;
    }

    void AbstractShape::addForceAt(const glm::dvec2& force, const glm::dvec2& at)
    {
        glm::dvec2 radius = at - _centroid;
        addAngularForce(Segment2D::cross(radius, force));
        addLinearForce(force);
    }

    void AbstractShape::applyLinearImpulse(const glm::dvec2& impulse)
    {
        if(_bodyType == EBodyType::DYNAMIC)
            _linearVelocity += impulse * _inverseMass;
    }

    void AbstractShape::applyAngularImpulse(const double& impulse)
    {
        if(_bodyType == EBodyType::DYNAMIC)
            _angularVelocity += impulse * _inverseMomentOfInertia;
    }

    void AbstractShape::applyImpulseAt(const glm::dvec2& impulse, const glm::dvec2& at)
    {
        glm::dvec2 radius = at - _centroid;
        applyAngularImpulse(Segment2D::cross(radius, impulse));
        applyLinearImpulse(impulse);
    }
}
