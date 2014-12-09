#ifndef PROPROOM3D_ABSTRACTSHAPE_H
#define PROPROOM3D_ABSTRACTSHAPE_H

#include <memory>
#include <vector>

#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

#include <DesignPattern/SpecificObserver.h>

#include "libPropRoom3D_global.h"


namespace prop3
{
class AbstractPropTeam;
class AbstractCostume;
class HardwareUpdate;
class Hardware;

    struct PROP3D_EXPORT Ray
    {
        Ray(const glm::dvec3& origin,
            const glm::dvec3& direction) :
            origin(origin),
            direction(direction)
        {}

        glm::dvec3 origin;
        glm::dvec3 direction;
    };

    struct PROP3D_EXPORT RaycastReport
    {
        RaycastReport(const glm::dvec3& position,
                      const glm::dvec3& normal,
                      double t) :
            position(position),
            normal(normal),
            t(t)
        {}

        glm::dvec3 position;
        glm::dvec3 normal;
        double t;
    };

    class PROP3D_EXPORT AbstractShape :
            public cellar::SpecificObserver<HardwareUpdate>
    {
    protected:
        AbstractShape(EPropType propType);

    public:
        virtual ~AbstractShape();

        // Identification
        PropId id() const;
        EPropType propType() const;

        // Visibility
        bool isVisible() const;
        void setIsVisible(bool isVisible);

        // Costume
        virtual const std::shared_ptr<AbstractCostume>& costume() const;
        virtual const void setCostume(const std::shared_ptr<AbstractCostume>& costume);

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
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const =0;


        // Constant attributes
        static const double INFINITE_INERTIA;
        static const glm::dmat3 INFINITE_MOMENT_OF_INERTIA;

    protected:
        // Cached attributes update
        virtual void updateTransformMatrix() =0;
        virtual void updateInertia() =0;


        // Attributes
        std::shared_ptr<AbstractCostume> _costume;
        std::shared_ptr<Hardware> _material;
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

        glm::dmat4 _transformMatrix;
        glm::dmat4 _transformMatrixInv;

    private:
        static PropId _assigneId_();
        static PropId _nextId_;

        PropId    _id;
        EPropType _propType;
        bool      _isVisible;
    };



    // IMPLEMENTATION //
    inline PropId AbstractShape::id() const
    {
        return _id;
    }

    inline EPropType AbstractShape::propType() const
    {
        return _propType;
    }

    inline bool AbstractShape::isVisible() const
    {
        return _isVisible;
    }

    inline void AbstractShape::setIsVisible(bool isVisible)
    {
        _isVisible = isVisible;
    }

    inline PropId AbstractShape::_assigneId_()
    {
        return _nextId_++;
    }
}

#endif // PROPROOM3D_ABSTRACTSHAPE_H
