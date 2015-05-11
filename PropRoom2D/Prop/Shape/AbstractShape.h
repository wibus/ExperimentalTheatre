#ifndef PROPROOM2D_ABSTRACTSHAPE_H
#define PROPROOM2D_ABSTRACTSHAPE_H

#include <memory>

#include <GLM/glm.hpp>

#include <CellarWorkbench/DesignPattern/SpecificObserver.h>

#include "../AbstractProp.h"


namespace prop2
{
    class AbstractTeam;
    class AbstractCostume;
    class HardwareUpdate;
    class Hardware;


    class PROP2D_EXPORT AbstractShape :
            public AbstractProp,
            public cellar::SpecificObserver<HardwareUpdate>
    {
    protected:
        AbstractShape(EPropType propType);

    public:
        virtual ~AbstractShape();

        // Costume
        virtual const AbstractCostume& abstractCostume() const =0;

        // Material
        virtual const std::shared_ptr<Hardware>& hardware() const;
        virtual void setHardware(const std::shared_ptr<Hardware>& hardware);
        virtual void notify(HardwareUpdate& msg);

        // Body type
        virtual EBodyType bodyType() const;
        virtual void setBodyType(const EBodyType& type);

        // Inertia
        virtual double mass() const;
        virtual double inverseMass() const;
        virtual double momentOfInertia() const;
        virtual double inverseMomentOfInertia() const;

        // Position
        virtual glm::dvec2 centroid() const;
        virtual void moveBy(const glm::dvec2& displacement);
        virtual void setCentroid(const glm::dvec2& position);

        // Linear velocity
        virtual glm::dvec2 linearVelocity() const;
        virtual void setLinearVelocity(const glm::dvec2& velocity);
        virtual void addLinearVelocity(const glm::dvec2& velocity);

        // Linear acceleration
        virtual glm::dvec2 linearAcceleration() const;
        virtual void setLinearAcceleration(const glm::dvec2& acceleration);
        virtual void addLinearAcceleration(const glm::dvec2& acceleration);

        // Linear friction
        virtual glm::dvec3 linearFrictionCoefficients() const;
        virtual void setLinearFrictionCoefficients(const glm::dvec3& coeffs);
        virtual void setLinearFrictionCoefficient(int order, double coeff);

        // Rotation
        virtual double angle() const;
        virtual void rotate(const double& angle);
        virtual void setAngle(const double& angle);

        // Angular velocity
        virtual double angularVelocity() const;
        virtual void setAngularVelocity(const double& velocity);
        virtual void addAngularVelocity(const double& velocity);

        // Angular acceleration
        virtual double angularAcceleration() const;
        virtual void setAngularAcceleration(const double& acceleration);
        virtual void addAngularAcceleration(const double& acceleration);

        // Angular friction
        virtual glm::dvec3 angularFrictionCoefficients() const;
        virtual void setAngularFrictionCoefficients(const glm::dvec3& coeffs);
        virtual void setAngularFrictionCoefficient(int order, double coeff);

        // Force
        virtual void addLinearForce(const glm::dvec2& force);
        virtual void addAngularForce(const double& force);
        virtual void addForceAt(const glm::dvec2& force, const glm::dvec2& at);

        // Impulse
        virtual void applyLinearImpulse(const glm::dvec2& impulse);
        virtual void applyAngularImpulse(const double& impulse);
        virtual void applyImpulseAt(const glm::dvec2& impulse, const glm::dvec2& at);

        // Transformation matrix
        virtual const glm::dmat3& transformMatrix() const;

        // Tests
        virtual bool contains(const glm::dvec2& point) const =0;
        virtual glm::dvec2 nearestSurface(const glm::dvec2& point) const =0;

        // Area
        virtual double computeArea() const = 0;


        // Constant attributes
        static const double INFINITE_INERTIA;

    protected:
        // Cached attributes update
        virtual void updateTransformMatrix() =0;
        virtual void updateInertia() =0;


        // Attributes
        std::shared_ptr<Hardware> _material;
        EBodyType _bodyType;

        double _inverseMass;
        double _inverseMomentOfInertia;

        glm::dvec2 _centroid;
        glm::dvec2 _linearVelocity;
        glm::dvec2 _linearAcceleration;
        glm::dvec3 _linearFirctionCoefficients;

        double _angle;
        double _angularVelocity;
        double _angularAcceleration;
        glm::dvec3 _angularFirctionCoefficients;

        glm::dmat3 _tranformMatrix;
    };
}

#endif // PROPROOM2D_ABSTRACTSHAPE_H
