#include "AbstractShape.h"

#include <cassert>


namespace prop2
{
    const real AbstractShape::INFINIT_INERTIA = real(0);

    AbstractShape::AbstractShape(PropType::Enum propType) :
        AbstractProp(propType),
        _bodyType(BodyType::DYNAMIC),
        _tranformMatrix(real(1.0)),
        _mass(INFINIT_INERTIA),
        _momentOfInertia(INFINIT_INERTIA),
        _density(real(1.0)),
        _friction(real(1.0)),
        _bounciness(real(1.0)),
        _perimeter(real(0.0)),
        _area(real(0.0)),
        _centroid(real(0.0), real(0.0)),
        _linearVelocity(real(0.0), real(0.0)),
        _linearAcceleration(real(0.0), real(0.0)),
        _angle(real(0.0)),
        _angularVelocity(real(0.0)),
        _angularAcceleration(real(0.0))
    {
    }

    AbstractShape::~AbstractShape()
    {
    }

    void AbstractShape::setBodyType(const BodyType::Enum& type)
    {
        _bodyType = type;
    }

    void AbstractShape::setDensity(const real& density)
    {
        _density = density;
        updateInertia();
    }

    void AbstractShape::setFriction(const real& friction)
    {
        _friction = friction;
    }

    void AbstractShape::setBounciness(const real& bounciness)
    {
        _bounciness = bounciness;
    }

    void AbstractShape::moveBy(const Vec2r& displacement)
    {
        _centroid += displacement;
        updateTranformMatrix();
    }

    void AbstractShape::setCentroid(const Vec2r& position)
    {
        _centroid = position;
        updateTranformMatrix();
    }

    void AbstractShape::setLinearVelocity(const Vec2r& velocity)
    {
        _linearVelocity = velocity;
    }

    void AbstractShape::addLinearVelocity(const Vec2r& velocity)
    {
        _linearVelocity += velocity;
    }

    void AbstractShape::setLinearAcceleration(const Vec2r& acceleration)
    {
        _linearAcceleration = acceleration;
    }

    void AbstractShape::addLinearAcceleration(const Vec2r& acceleration)
    {
        _linearAcceleration += acceleration;
    }

    void AbstractShape::addLinearForce(const Vec2r& force)
    {
        if(_mass != INFINIT_INERTIA)
        {
            _linearAcceleration += force / _mass;
        }
    }

    void AbstractShape::rotate(const real& angle)
    {
        _angle += angle;
        updateTranformMatrix();
    }

    void AbstractShape::setAngle(const real& angle)
    {
        _angle = angle;
        updateTranformMatrix();
    }

    void AbstractShape::setAngularVelocity(const real& velocity)
    {
        _angularVelocity = velocity;
    }

    void AbstractShape::addAngularVelocity(const real& velocity)
    {
        _angularVelocity += velocity;
    }

    void AbstractShape::setAngularAcceleration(const real& acceleration)
    {
        _angularAcceleration = acceleration;
    }

    void AbstractShape::addAngularAcceleration(const real& acceleration)
    {
        _angularAcceleration += acceleration;
    }

    void AbstractShape::addAngularForce(const real& force)
    {
        if(_momentOfInertia != INFINIT_INERTIA)
        {
            _angularAcceleration += force / _momentOfInertia;
        }
    }

    void AbstractShape::addForceAt(const Vec2r& force, const Vec2r& at)
    {
        Vec2r radius = at - _centroid;
        addAngularForce(radius ^ force);
        addLinearForce(force);
    }
}
