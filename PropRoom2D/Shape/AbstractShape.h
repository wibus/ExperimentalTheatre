#ifndef PROPROOM2D_ABSTRACTSHAPE_H
#define PROPROOM2D_ABSTRACTSHAPE_H

#include <memory>

#include "libPropRoom2D_global.h"
#include "AbstractProp.h"


namespace prop2
{
class AbstractPropTeam;
class AbstractCostume;


    class PROP2D_EXPORT AbstractShape : public AbstractProp
    {
    protected:
        AbstractShape(PropType::Enum propType);

    public:
        virtual ~AbstractShape();

        // Costume
        virtual const AbstractCostume& abstractCostume() const =0;

        // Getters
        BodyType::Enum bodyType() const;

        const Mat3r& transformMatrix() const;

        real mass() const;
        real inverseMass() const;
        real momentOfInertia() const;
        real inverseMomentOfInertia() const;
        real density() const;
        real staticFrictionCoefficient() const;
        real dynamicFrictionCoefficient() const;
        real bounciness() const;
        real perimeter() const;
        real area() const;

        Vec2r centroid() const;
        Vec2r linearVelocity() const;
        Vec2r linearAcceleration() const;

        real angle() const;
        real angularVelocity() const;
        real angularAcceleration() const;


        // Setters
        virtual void setBodyType(const BodyType::Enum& type);

        virtual void setDensity(const real& density);
        virtual void setStaticFrictionCoefficient(const real& us);
        virtual void setDynamicFrictionCoefficient(const real& ud);
        virtual void setBounciness(const real& bounciness);

        virtual void moveBy(const Vec2r& displacement);
        virtual void setCentroid(const Vec2r& position);
        virtual void setLinearVelocity(const Vec2r& velocity);
        virtual void addLinearVelocity(const Vec2r& velocity);
        virtual void setLinearAcceleration(const Vec2r& acceleration);
        virtual void addLinearAcceleration(const Vec2r& acceleration);
        virtual void addLinearForce(const Vec2r& force);
        virtual void applyLinearImpulse(const Vec2r& impulse);

        virtual void rotate(const real& angle);
        virtual void setAngle(const real& angle);
        virtual void setAngularVelocity(const real& velocity);
        virtual void addAngularVelocity(const real& velocity);
        virtual void setAngularAcceleration(const real& acceleration);
        virtual void addAngularAcceleration(const real& acceleration);
        virtual void addAngularForce(const real& force);
        virtual void applyAngularImpulse(const real& impulse);

        virtual void addForceAt(const Vec2r& force, const Vec2r& at);
        virtual void applyImpulseAt(const Vec2r& impulse, const Vec2r& at);


        // Tests
        virtual bool contains(const Vec2r& point) const =0;
        virtual Vec2r nearestSurface(const Vec2r& point) const =0;


        // Constant attributes
        static const real INFINITE_DENSITY;

    protected:
        // Computations
        virtual void updateTranformMatrix() =0;
        virtual void updatePerimeter() =0;
        virtual void updateArea() =0;
        virtual void updateInertia() =0;


        // Attributes
        BodyType::Enum _bodyType;

        Mat3r _tranformMatrix;

        real _inverseMass;
        real _inverseMomentOfInertia;
        real _density;
        real _staticFrictionCoefficient;
        real _dynamicFrictionCoefficient;
        real _bounciness;
        real _perimeter;
        real _area;

        Vec2r _centroid;
        Vec2r _linearVelocity;
        Vec2r _linearAcceleration;

        real _angle;
        real _angularVelocity;
        real _angularAcceleration;
    };



    // IMPLEMENTATION //
    inline BodyType::Enum AbstractShape::bodyType() const
    {
        return _bodyType;
    }

    inline const Mat3r& AbstractShape::transformMatrix() const
    {
        return _tranformMatrix;
    }

    inline real AbstractShape::mass() const
    {
        return _inverseMass ?
                    real(1.0)/_inverseMass :
                    real(0.0);
    }

    inline real AbstractShape::inverseMass() const
    {
        return _inverseMass;
    }

    inline real AbstractShape::momentOfInertia() const
    {
        return _inverseMomentOfInertia ?
                    real(1.0)/_inverseMomentOfInertia :
                    real(0.0);
    }

    inline real AbstractShape::inverseMomentOfInertia() const
    {
        return _inverseMomentOfInertia;
    }

    inline real AbstractShape::density() const
    {
        return _density;
    }

    inline real AbstractShape::staticFrictionCoefficient() const
    {
        return _staticFrictionCoefficient;
    }

    inline real AbstractShape::dynamicFrictionCoefficient() const
    {
        return _dynamicFrictionCoefficient;
    }

    inline real AbstractShape::bounciness() const
    {
        return _bounciness;
    }

    inline real AbstractShape::perimeter() const
    {
        return _perimeter;
    }

    inline real AbstractShape::area() const
    {
        return _area;
    }

    inline Vec2r AbstractShape::centroid() const
    {
        return _centroid;
    }

    inline Vec2r AbstractShape::linearVelocity() const
    {
        return _linearVelocity;
    }

    inline Vec2r AbstractShape::linearAcceleration() const
    {
        return _linearAcceleration;
    }

    inline real AbstractShape::angle() const
    {
        return _angle;
    }

    inline real AbstractShape::angularVelocity() const
    {
        return _angularVelocity;
    }

    inline real AbstractShape::angularAcceleration() const
    {
        return _angularAcceleration;
    }
}

#endif // PROPROOM2D_ABSTRACTSHAPE_H
