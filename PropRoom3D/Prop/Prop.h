#ifndef PROPROOM3D_PROP_H
#define PROPROOM3D_PROP_H

#include <memory>
#include <vector>

#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

#include <DesignPattern/SpecificObserver.h>

#include "libPropRoom3D_global.h"
#include "Volume/Volume.h"


namespace prop3
{
class AbstractPropTeam;
class AbstractCostume;
class HardwareUpdate;
class Hardware;

    class PROP3D_EXPORT Prop :
            public cellar::SpecificObserver<HardwareUpdate>
    {
    public:
        Prop();
        virtual ~Prop();

        // Identification
        PropId id() const;

        // Visibility
        bool isVisible() const;
        virtual void setIsVisible(bool isVisible);

        // Volume
        std::shared_ptr<Volume> equation() const;
        virtual void setEquation(const std::shared_ptr<Volume>& eq);

        // Costume
        const std::shared_ptr<AbstractCostume>& costume() const;
        virtual const void setCostume(const std::shared_ptr<AbstractCostume>& costume);

        // Material
        const std::shared_ptr<Hardware>& hardware() const;
        virtual void setHardware(const std::shared_ptr<Hardware>& hardware);
        virtual void notify(HardwareUpdate& msg);

        // Body type
        EBodyType bodyType() const;
        virtual void setBodyType(const EBodyType& type);

        // Inertia
        double mass() const;
        double inverseMass() const;
        glm::dmat3 momentOfInertia() const;
        glm::dmat3 inverseMomentOfInertia() const;

        // Position
        glm::dvec3 centroid() const;
        virtual void moveBy(const glm::dvec3& displacement);
        virtual void setCentroid(const glm::dvec3& position);

        // Linear velocity
        glm::dvec3 linearVelocity() const;
        virtual void setLinearVelocity(const glm::dvec3& velocity);
        virtual void addLinearVelocity(const glm::dvec3& velocity);

        // Linear acceleration
        glm::dvec3 linearAcceleration() const;
        virtual void setLinearAcceleration(const glm::dvec3& acceleration);
        virtual void addLinearAcceleration(const glm::dvec3& acceleration);

        // Linear friction
        glm::dvec3 linearFrictionCoefficients() const;
        virtual void setLinearFrictionCoefficients(const glm::dvec3& coeffs);
        virtual void setLinearFrictionCoefficient(int order, double coeff);

        // Rotation
        glm::dquat angle() const;
        virtual void rotate(const glm::dquat& angle);
        virtual void setAngle(const glm::dquat& angle);

        // Angular velocity
        glm::dquat angularVelocity() const;
        virtual void setAngularVelocity(const glm::dquat& velocity);
        virtual void addAngularVelocity(const glm::dquat& velocity);

        // Angular acceleration
        glm::dquat angularAcceleration() const;
        virtual void setAngularAcceleration(const glm::dquat& acceleration);
        virtual void addAngularAcceleration(const glm::dquat& acceleration);

        // Angular friction
        glm::dvec3 angularFrictionCoefficients() const;
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
        virtual bool contains(const glm::dvec3& point) const;
        virtual glm::dvec3 nearestSurface(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;


        // Constant attributes
        static const double INFINITE_INERTIA;
        static const glm::dmat3 INFINITE_MOMENT_OF_INERTIA;

    protected:
        // Cached attributes update
        virtual void updateTransformMatrix();
        virtual void updateInertia();


        // Attributes
        std::shared_ptr<Volume> _eq;
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
        bool      _isVisible;
    };



    // IMPLEMENTATION //
    inline PropId Prop::id() const
    {
        return _id;
    }

    inline bool Prop::isVisible() const
    {
        return _isVisible;
    }

    inline std::shared_ptr<Volume> Prop::equation() const
    {
        return _eq;
    }

    inline const std::shared_ptr<AbstractCostume>& Prop::costume() const
    {
        return _costume;
    }

    inline const std::shared_ptr<Hardware>& Prop::hardware() const
    {
        return _material;
    }

    inline EBodyType Prop::bodyType() const
    {
        return _bodyType;
    }

    inline double Prop::mass() const
    {
        return _bodyType ==  EBodyType::DYNAMIC && _invMass ?
            1.0 / _invMass :
            INFINITE_INERTIA;
    }

    inline double Prop::inverseMass() const
    {
        return _bodyType ==  EBodyType::DYNAMIC ?
            _invMass :
            INFINITE_INERTIA;
    }

    inline glm::dmat3 Prop::momentOfInertia() const
    {
        return _bodyType ==  EBodyType::DYNAMIC && _invMomentOfInertia[0][0] != 0.0?
            glm::inverse(_invMomentOfInertia) :
            INFINITE_MOMENT_OF_INERTIA;
    }

    inline glm::dmat3 Prop::inverseMomentOfInertia() const
    {
        return _bodyType ==  EBodyType::DYNAMIC ?
            _invMomentOfInertia :
            INFINITE_MOMENT_OF_INERTIA;
    }

    inline glm::dvec3 Prop::centroid() const
    {
        return _centroid;
    }

    inline glm::dvec3 Prop::linearVelocity() const
    {
        return _linearVelocity;
    }

    inline glm::dvec3 Prop::linearAcceleration() const
    {
        return _linearAcceleration;
    }

    inline glm::dvec3 Prop::linearFrictionCoefficients() const
    {
        return _linearFirctionCoefficients;
    }

    inline glm::dquat Prop::angle() const
    {
        return _angle;
    }

    inline glm::dquat Prop::angularVelocity() const
    {
        return _angularVelocity;
    }

    inline glm::dquat Prop::angularAcceleration() const
    {
        return _angularAcceleration;
    }

    inline glm::dvec3 Prop::angularFrictionCoefficients() const
    {
        return _angularFirctionCoefficients;
    }

    inline PropId Prop::_assigneId_()
    {
        return _nextId_++;
    }
}

#endif // PROPROOM3D_PROP_H
