#ifndef PROPROOM3D_VOLUME_H
#define PROPROOM3D_VOLUME_H

#include <memory>
#include <vector>

#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
class Ray;
class RaycastReport;

    enum class EPointPosition
    {
        OUT,
        ON,
        IN
    };

    struct PROP3D_EXPORT Transform
    {
        Transform(const glm::dmat4& transform);
        Transform(double scale ,
                  const glm::dquat rotation,
                  const glm::dvec3 translation);

        const glm::dmat4& mat() const { return _mat; }
        const glm::dmat4& inv() const
        {
            if(_isInvComputed) return _mat;
            return _inv = glm::inverse(_mat);
        }

    private:
        glm::dmat4 _mat;
        mutable glm::dmat4 _inv;
        bool _isInvComputed;
    };

    class PROP3D_EXPORT Volume
    {
    protected:
        Volume();

    public:
        virtual ~Volume();

        virtual std::shared_ptr<Volume> clone() const = 0;

        virtual void transform(const Transform& transform) = 0;

        EPointPosition isIn(double x, double y, double z) const;
        virtual EPointPosition isIn(const glm::dvec3& point) const = 0;

        double signedDistance(double x, double y, double z) const;
        virtual double signedDistance(const glm::dvec3& point) const = 0;

        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const =0;
    };



    class PROP3D_EXPORT VolumeNot : public Volume
    {
        friend std::shared_ptr<Volume> operator!(
                const std::shared_ptr<Volume>&);

        VolumeNot(const VolumeNot& v) = delete;
        VolumeNot& operator=(const VolumeNot&) = delete;

    public:
        VolumeNot(const std::shared_ptr<Volume>& eq);

        virtual std::shared_ptr<Volume> clone() const;
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;

    private:
        std::shared_ptr<Volume> _eq;
    };



    class PROP3D_EXPORT VolumeOr : public Volume
    {
        friend std::shared_ptr<Volume> operator| (
                const std::shared_ptr<Volume>&,
                const std::shared_ptr<Volume>&);

        VolumeOr(const VolumeOr& v) = delete;
        VolumeOr& operator=(const VolumeOr&) = delete;

    public:
        VolumeOr(const std::vector<std::shared_ptr<Volume>>& eqs);

        virtual std::shared_ptr<Volume> clone() const;
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;

    private:
        void addVolume(const std::shared_ptr<Volume>& volume);

        std::vector<std::shared_ptr<Volume>> _eqs;
    };



    class PROP3D_EXPORT VolumeAnd : public Volume
    {
        friend std::shared_ptr<Volume> operator& (
                const std::shared_ptr<Volume>&,
                const std::shared_ptr<Volume>&);

        VolumeAnd(const VolumeAnd& v) = delete;
        VolumeAnd& operator=(const VolumeAnd&) = delete;

    public:
        VolumeAnd(const std::vector<std::shared_ptr<Volume>>& eqs);

        virtual std::shared_ptr<Volume> clone() const;
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;

    private:
        void addVolume(const std::shared_ptr<Volume>& volume);

        std::vector<std::shared_ptr<Volume>> _eqs;
    };


    // Logical operators overloading
    std::shared_ptr<Volume> operator!(
            const std::shared_ptr<Volume>& eq);

    std::shared_ptr<Volume> operator| (
            const std::shared_ptr<Volume>& eq1,
            const std::shared_ptr<Volume>& eq2);

    std::shared_ptr<Volume> operator& (
            const std::shared_ptr<Volume>& eq1,
            const std::shared_ptr<Volume>& eq2);



    // IMPLEMENTATION //
    inline EPointPosition Volume::isIn(double x, double y, double z) const
    {
        return isIn(glm::dvec3(x, y, z));
    }

    inline double Volume::signedDistance(double x, double y, double z) const
    {
        return signedDistance(glm::dvec3(x, y, z));
    }

    inline void VolumeOr::addVolume(const std::shared_ptr<Volume>& volume)
    {
        _eqs.push_back(volume);
    }

    inline void VolumeAnd::addVolume(const std::shared_ptr<Volume>& volume)
    {
        _eqs.push_back(volume);
    }
}

#endif // PROPROOM3D_VOLUME_H
