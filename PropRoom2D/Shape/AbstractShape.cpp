#include "AbstractShape.h"

#include <cassert>


namespace prop2
{
    const real AbstractShape::INFINITE_DENSITY = real(0);

    AbstractShape::AbstractShape(PropType::Enum propType) :
        AbstractProp(propType),
        _bodyType(BodyType::DYNAMIC),
        _tranformMatrix(real(1.0)),
        _inverseMass(real(0.0)),
        _inverseMomentOfInertia(real(0.0)),
        _density(real(1.0)),
        _staticFrictionCoefficient(real(1.0)),
        _dynamicFrictionCoefficient(real(1.0)),
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
        if(_bodyType != type)
        {
            _bodyType = type;
            updateInertia();
        }
    }

    void AbstractShape::setDensity(const real& density)
    {
        if(_density != density)
        {
            _density = density;
            updateInertia();
        }
    }

    void AbstractShape::setStaticFrictionCoefficient(const real& us)
    {
        _staticFrictionCoefficient = us;
    }

    void AbstractShape::setDynamicFrictionCoefficient(const real& ud)
    {
        _dynamicFrictionCoefficient = ud;
    }

    void AbstractShape::setBounciness(const real& bounciness)
    {
        _bounciness = bounciness;
    }

    void AbstractShape::moveBy(const Vec2r& displacement)
    {
        if(displacement != Vec2r())
        {
            _centroid += displacement;
            updateTranformMatrix();
        }
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
        _linearAcceleration += force * _inverseMass;
    }

    void AbstractShape::applyLinearImpulse(const Vec2r& impulse)
    {
        _linearVelocity += impulse * _inverseMass;
    }

    void AbstractShape::rotate(const real& angle)
    {
        if(angle != real(0.0))
        {
            _angle += angle;
            updateTranformMatrix();
        }
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
        _angularAcceleration += force * _inverseMomentOfInertia;
    }

    void AbstractShape::applyAngularImpulse(const real& impulse)
    {
        _angularVelocity += impulse * _inverseMomentOfInertia;
    }

    void AbstractShape::addForceAt(const Vec2r& force, const Vec2r& at)
    {
        Vec2r radius = at - centroid();
        addAngularForce(cross(radius, force));
        addLinearForce(force);
    }

    void AbstractShape::applyImpulseAt(const Vec2r& impulse, const Vec2r& at)
    {
        Vec2r radius = at - centroid();
        applyAngularImpulse(cross(radius, impulse));
        applyLinearImpulse(impulse);
    }
}
