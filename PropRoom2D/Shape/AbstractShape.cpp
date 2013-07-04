#include "AbstractShape.h"
#include "Material/Material.h"

#include <cassert>


namespace prop2
{

    const real AbstractShape::INFINITE_INERTIA = real(0);

    AbstractShape::AbstractShape(PropType::Enum propType) :
        AbstractProp(propType),
        _material(),
        _bodyType(BodyType::GRAPHIC),
        _tranformMatrix(real(1.0)),
        _inverseMass(INFINITE_INERTIA),
        _inverseMomentOfInertia(INFINITE_INERTIA),
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
        if(_material)
        {
            _material->unregisterObserver(*this);
        }
    }

    const std::shared_ptr<Material>& AbstractShape::material() const
    {
        return _material;
    }

    void AbstractShape::setMaterial(const std::shared_ptr<Material>& material)
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

    void AbstractShape::notify(MaterialUpdate& msg)
    {
        if(msg.type == MaterialUpdate::DENSITY)
        {
            updateInertia();
        }
    }

    BodyType::Enum AbstractShape::bodyType() const
    {
        return _bodyType;
    }

    void AbstractShape::setBodyType(const BodyType::Enum& type)
    {
        if(_bodyType != type)
        {
            _bodyType = type;
        }
    }

    const Mat3r& AbstractShape::transformMatrix() const
    {
        return _tranformMatrix;
    }

    real AbstractShape::mass() const
    {
        return _bodyType ==  BodyType::DYNAMIC && _inverseMass ?
            real(1)/_inverseMass :
            INFINITE_INERTIA;
    }

    real AbstractShape::inverseMass() const
    {
        return _bodyType ==  BodyType::DYNAMIC ?
            _inverseMass :
            INFINITE_INERTIA;
    }

    real AbstractShape::momentOfInertia() const
    {
        return _bodyType ==  BodyType::DYNAMIC && _inverseMomentOfInertia ?
            real(1)/_inverseMomentOfInertia :
            INFINITE_INERTIA;
    }

    real AbstractShape::inverseMomentOfInertia() const
    {
        return _bodyType ==  BodyType::DYNAMIC ?
            _inverseMomentOfInertia :
            INFINITE_INERTIA;
    }

    Vec2r AbstractShape::centroid() const
    {
        return _centroid;
    }

    void AbstractShape::moveBy(const Vec2r& displacement)
    {
        if(displacement)
        {
            _centroid += displacement;
            updateTranformMatrix();
        }
    }

    void AbstractShape::setCentroid(const Vec2r& position)
    {
        if(_centroid != position)
        {
            _centroid = position;
            updateTranformMatrix();
        }
    }

    Vec2r AbstractShape::linearVelocity() const
    {
        return _linearVelocity;
    }

    void AbstractShape::setLinearVelocity(const Vec2r& velocity)
    {
        _linearVelocity = velocity;
    }

    void AbstractShape::addLinearVelocity(const Vec2r& velocity)
    {
        _linearVelocity += velocity;
    }

    Vec2r AbstractShape::linearAcceleration() const
    {
        return _linearAcceleration;
    }

    void AbstractShape::setLinearAcceleration(const Vec2r& acceleration)
    {
        _linearAcceleration = acceleration;
    }

    void AbstractShape::addLinearAcceleration(const Vec2r& acceleration)
    {
        _linearAcceleration += acceleration;
    }

    real AbstractShape::angle() const
    {
        return _angle;
    }

    void AbstractShape::rotate(const real& angle)
    {
        if(angle)
        {
            _angle += angle;
            updateTranformMatrix();
        }
    }

    void AbstractShape::setAngle(const real& angle)
    {
        if(_angle != angle)
        {
            _angle = angle;
            updateTranformMatrix();
        }
    }

    real AbstractShape::angularVelocity() const
    {
        return _angularVelocity;
    }

    void AbstractShape::setAngularVelocity(const real& velocity)
    {
        _angularVelocity = velocity;
    }

    void AbstractShape::addAngularVelocity(const real& velocity)
    {
        _angularVelocity += velocity;
    }

    real AbstractShape::angularAcceleration() const
    {
        return _angularAcceleration;
    }

    void AbstractShape::setAngularAcceleration(const real& acceleration)
    {
        _angularAcceleration = acceleration;
    }

    void AbstractShape::addAngularAcceleration(const real& acceleration)
    {
        _angularAcceleration += acceleration;
    }

    void AbstractShape::addLinearForce(const Vec2r& force)
    {
        if(_bodyType == BodyType::DYNAMIC)
            _linearAcceleration += force * _inverseMass;
    }

    void AbstractShape::addAngularForce(const real& force)
    {
        if(_bodyType == BodyType::DYNAMIC)
            _angularAcceleration += force * _inverseMomentOfInertia;
    }

    void AbstractShape::addForceAt(const Vec2r& force, const Vec2r& at)
    {
        Vec2r radius = at - _centroid;
        addAngularForce(cross(radius, force));
        addLinearForce(force);
    }

    void AbstractShape::applyLinearImpulse(const Vec2r& impulse)
    {
        if(_bodyType == BodyType::DYNAMIC)
            _linearVelocity += impulse * _inverseMass;
    }

    void AbstractShape::applyAngularImpulse(const real& impulse)
    {
        if(_bodyType == BodyType::DYNAMIC)
            _angularVelocity += impulse * _inverseMomentOfInertia;
    }

    void AbstractShape::applyImpulseAt(const Vec2r& impulse, const Vec2r& at)
    {
        Vec2r radius = at - _centroid;
        applyAngularImpulse(cross(radius, impulse));
        applyLinearImpulse(impulse);
    }
}
