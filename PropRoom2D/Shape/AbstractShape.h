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
        real momentOfInertia() const;
        real density() const;
        real friction() const;
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
        virtual void setFriction(const real& friction);
        virtual void setBounciness(const real& bounciness);

        virtual void moveBy(const Vec2r& displacement);
        virtual void setCentroid(const Vec2r& position);
        virtual void setLinearVelocity(const Vec2r& velocity);
        virtual void addLinearVelocity(const Vec2r& velocity);
        virtual void setLinearAcceleration(const Vec2r& acceleration);
        virtual void addLinearAcceleration(const Vec2r& acceleration);
        virtual void addLinearForce(const Vec2r& force);

        virtual void rotate(const real& angle);
        virtual void setAngle(const real& angle);
        virtual void setAngularVelocity(const real& velocity);
        virtual void addAngularVelocity(const real& velocity);
        virtual void setAngularAcceleration(const real& acceleration);
        virtual void addAngularAcceleration(const real& acceleration);
        virtual void addAngularForce(const real& force);

        virtual void addForceAt(const Vec2r& force, const Vec2r& at);


        // Tests
        virtual bool contains(const Vec2r& point) const =0;
        virtual bool intersects(const Segment2Dr& line) const =0;


        // Constant attributes
        static const real INFINIT_INERTIA;


    protected:
        // Computations
        virtual void updateTranformMatrix() =0;
        virtual void updatePerimeter() =0;
        virtual void updateArea() =0;
        virtual void updateInertia() =0;


        // Attributes
        BodyType::Enum _bodyType;

        Mat3r _tranformMatrix;

        real _mass;
        real _momentOfInertia;
        real _density;
        real _friction;
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
        return _mass;
    }

    inline real AbstractShape::momentOfInertia() const
    {
        return _momentOfInertia;
    }

    inline real AbstractShape::density() const
    {
        return _density;
    }

    inline real AbstractShape::friction() const
    {
        return _friction;
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
