#ifndef PROPROOM2D_ABSTRACTSHAPE_H
#define PROPROOM2D_ABSTRACTSHAPE_H

#include <memory>

#include <DesignPattern/SpecificObserver.h>

#include "libPropRoom2D_global.h"
#include "AbstractProp.h"


namespace prop2
{
class AbstractPropTeam;
class AbstractCostume;
class MaterialUpdate;
class Material;


    class PROP2D_EXPORT AbstractShape :
            public AbstractProp,
            public cellar::SpecificObserver<MaterialUpdate>
    {
    protected:
        AbstractShape(PropType::Enum propType);

    public:
        virtual ~AbstractShape();

        // Costume
        virtual const AbstractCostume& abstractCostume() const =0;

        // Material
        virtual const std::shared_ptr<Material>& material() const;
        virtual void setMaterial(const std::shared_ptr<Material>& material);
        virtual void notify(MaterialUpdate& msg);

        // Body type
        virtual BodyType::Enum bodyType() const;
        virtual void setBodyType(const BodyType::Enum& type);

        // Inertia
        virtual real mass() const;
        virtual real inverseMass() const;
        virtual real momentOfInertia() const;
        virtual real inverseMomentOfInertia() const;

        // Position
        virtual Vec2r centroid() const;
        virtual void moveBy(const Vec2r& displacement);
        virtual void setCentroid(const Vec2r& position);

        // Linear velocity
        virtual Vec2r linearVelocity() const;
        virtual void setLinearVelocity(const Vec2r& velocity);
        virtual void addLinearVelocity(const Vec2r& velocity);

        // Linear acceleration
        virtual Vec2r linearAcceleration() const;
        virtual void setLinearAcceleration(const Vec2r& acceleration);
        virtual void addLinearAcceleration(const Vec2r& acceleration);

        // Linear friction
        virtual Vec3r linearFrictionCoefficients() const;
        virtual void setLinearFrictionCoefficients(const Vec3r& coeffs);
        virtual void setLinearFrictionCoefficient(int order, real coeff);

        // Rotation
        virtual real angle() const;
        virtual void rotate(const real& angle);
        virtual void setAngle(const real& angle);

        // Angular velocity
        virtual real angularVelocity() const;
        virtual void setAngularVelocity(const real& velocity);
        virtual void addAngularVelocity(const real& velocity);

        // Angular acceleration
        virtual real angularAcceleration() const;
        virtual void setAngularAcceleration(const real& acceleration);
        virtual void addAngularAcceleration(const real& acceleration);

        // Angular friction
        virtual Vec3r angularFrictionCoefficients() const;
        virtual void setAngularFrictionCoefficients(const Vec3r& coeffs);
        virtual void setAngularFrictionCoefficient(int order, real coeff);

        // Force
        virtual void addLinearForce(const Vec2r& force);
        virtual void addAngularForce(const real& force);
        virtual void addForceAt(const Vec2r& force, const Vec2r& at);

        // Impulse
        virtual void applyLinearImpulse(const Vec2r& impulse);
        virtual void applyAngularImpulse(const real& impulse);
        virtual void applyImpulseAt(const Vec2r& impulse, const Vec2r& at);

        // Transformation matrix
        virtual const Mat3r& transformMatrix() const;

        // Tests
        virtual bool contains(const Vec2r& point) const =0;
        virtual Vec2r nearestSurface(const Vec2r& point) const =0;

        // Area
        virtual real computeArea() const = 0;


        // Constant attributes
        static const real INFINITE_INERTIA;

    protected:
        // Cached attributes update
        virtual void updateTranformMatrix() =0;
        virtual void updateInertia() =0;


        // Attributes
        std::shared_ptr<Material> _material;
        BodyType::Enum _bodyType;

        real _inverseMass;
        real _inverseMomentOfInertia;

        Vec2r _centroid;
        Vec2r _linearVelocity;
        Vec2r _linearAcceleration;
        Vec3r _linearFirctionCoefficients;

        real _angle;
        real _angularVelocity;
        real _angularAcceleration;
        Vec3r _angularFirctionCoefficients;

        Mat3r _tranformMatrix;
    };
}

#endif // PROPROOM2D_ABSTRACTSHAPE_H
