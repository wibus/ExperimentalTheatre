#ifndef PROPROOM3D_ABSTRACTSHAPE_H
#define PROPROOM3D_ABSTRACTSHAPE_H

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <DesignPattern/SpecificObserver.h>

#include "libPropRoom3D_global.h"
#include "AbstractProp.h"


namespace prop3
{
class AbstractPropTeam;
class AbstractCostume;
class MaterialUpdate;
class Material;


    class PROP3D_EXPORT AbstractShape :
            public AbstractProp,
            public cellar::SpecificObserver<MaterialUpdate>
    {
    protected:
        AbstractShape(EPropType propType);

    public:
        virtual ~AbstractShape();

        // Costume
        virtual const AbstractCostume& abstractCostume() const =0;

        // Material
        virtual const std::shared_ptr<Material>& material() const;
        virtual void setMaterial(const std::shared_ptr<Material>& material);
        virtual void notify(MaterialUpdate& msg);

        // Body type
        virtual EBodyType bodyType() const;
        virtual void setBodyType(const EBodyType& type);

        // Inertia
        virtual double mass() const;
        virtual double inverseMass() const;
        virtual glm::dmat3 momentOfInertia() const;
        virtual glm::dmat3 inverseMomentOfInertia() const;

        // Position
        virtual glm::dvec3 centroid() const;
        virtual void moveBy(const glm::dvec3& displacement);
        virtual void setCentroid(const glm::dvec3& position);

        // Linear velocity
        virtual glm::dvec3 linearVelocity() const;
        virtual void setLinearVelocity(const glm::dvec3& velocity);
        virtual void addLinearVelocity(const glm::dvec3& velocity);

        // Linear acceleration
        virtual glm::dvec3 linearAcceleration() const;
        virtual void setLinearAcceleration(const glm::dvec3& acceleration);
        virtual void addLinearAcceleration(const glm::dvec3& acceleration);

        // Linear friction
        virtual glm::dvec3 linearFrictionCoefficients() const;
        virtual void setLinearFrictionCoefficients(const glm::dvec3& coeffs);
        virtual void setLinearFrictionCoefficient(int order, double coeff);

        // Rotation
        virtual glm::dquat angle() const;
        virtual void rotate(const glm::dquat& angle);
        virtual void setAngle(const glm::dquat& angle);

        // Angular velocity
        virtual glm::dquat angularVelocity() const;
        virtual void setAngularVelocity(const glm::dquat& velocity);
        virtual void addAngularVelocity(const glm::dquat& velocity);

        // Angular acceleration
        virtual glm::dquat angularAcceleration() const;
        virtual void setAngularAcceleration(const glm::dquat& acceleration);
        virtual void addAngularAcceleration(const glm::dquat& acceleration);

        // Angular friction
        virtual glm::dvec3 angularFrictionCoefficients() const;
        virtual void setAngularFrictionCoefficients(const glm::dvec3& coeffs);
        virtual void setAngularFrictionCoefficient(int order, double coeff);

        // Force
        virtual void addLinearForce(const glm::dvec3& force);
        virtual void addAngularForce(const glm::dvec3& force);
        virtual void addForceAt(const glm::dvec3& force, const glm::dvec3& at);

        // Impulse
        virtual void applyLinearImpulse(const glm::dvec3& impulse);
        virtual void applyAngularImpulse(const glm::dvec3& impulse);
        virtual void applyImpulseAt(const glm::dvec3& impulse, const glm::dvec3& at);

        // Tests
        virtual bool contains(const glm::dvec3& point) const =0;
        virtual glm::dvec3 nearestSurface(const glm::dvec3& point) const =0;

        // Area
        virtual double computeArea() const = 0;


        // Constant attributes
        static const double INFINITE_INERTIA;
        static const glm::dmat3 INFINITE_MOMENT_OF_INERTIA;

    protected:
        // Cached attributes update
        virtual void updateTransformMatrix() =0;
        virtual void updateInertia() =0;


        // Attributes
        std::shared_ptr<Material> _material;
        EBodyType _bodyType;

        double _invMass;
        glm::dmat3 _invMomentOfInertia;
        glm::dmat3 _invRotMomentOfInertia;

        glm::dvec3 _centroid;
        glm::dvec3 _linearVelocity;
        glm::dvec3 _linearAcceleration;
        glm::dvec3 _linearFirctionCoefficients;

        glm::dquat _angle;
        glm::dquat _angularVelocity;
        glm::dquat _angularAcceleration;
        glm::dvec3 _angularFirctionCoefficients;

        glm::dmat3 _rotationMatrix;
        glm::dmat3 _invRotationMatrix;
        glm::dmat3 _transformMatrix;
    };
}

#endif // PROPROOM3D_ABSTRACTSHAPE_H
