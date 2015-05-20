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

    class Volume;
    typedef std::shared_ptr<Volume> pVol;

    enum class EPointPosition
    {
        OUT,
        ON,
        IN
    };


    // Logical operators overloading
    // Ghost surface
    pVol operator~ (const pVol& eq);
    // Invert sides
    pVol operator! (const pVol& eq);
    // Volume union
    pVol operator| (const pVol& eq1, const pVol& eq2);
    // Volume intersection
    pVol operator& (const pVol& eq1, const pVol& eq2);


    struct PROP3D_EXPORT Transform
    {
        Transform(double scale ,
                  const glm::dquat& rotation,
                  const glm::dvec3& translation);

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

        virtual void transform(const Transform& transform) = 0;

        EPointPosition isIn(double x, double y, double z) const;
        virtual EPointPosition isIn(const glm::dvec3& point) const = 0;

        double signedDistance(double x, double y, double z) const;
        virtual double signedDistance(const glm::dvec3& point) const = 0;

        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const =0;
        virtual bool intersects(const Ray& ray) = 0;
    };


    // Logical volumes
    class PROP3D_EXPORT VolumeGhost : public Volume
    {
        friend pVol operator~ (const pVol&);
        VolumeGhost(const pVol& eq);

    public:
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;
        virtual bool intersects(const Ray& ray);

    private:
        pVol _eq;
    };


    class PROP3D_EXPORT VolumeNot : public Volume
    {
        friend pVol operator! (const pVol&);
        VolumeNot(const pVol& eq);

    public:
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;
        virtual bool intersects(const Ray& ray);

    private:
        pVol _eq;
    };


    class PROP3D_EXPORT VolumeOr : public Volume
    {
        friend pVol operator| (const pVol&, const pVol&);
        VolumeOr(const std::vector<pVol>& eqs);

    public:
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;
        virtual bool intersects(const Ray& ray);

    private:
        void add(const pVol& volume);
        virtual bool raycast(const Ray& ray, std::vector<RaycastReport>& reports, bool isTest) const;

        std::vector<pVol> _eqs;
    };


    class PROP3D_EXPORT VolumeAnd : public Volume
    {
        friend pVol operator& (const pVol&, const pVol&);
        VolumeAnd(const std::vector<pVol>& eqs);

    public:
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;
        virtual bool intersects(const Ray& ray);

    private:
        void add(const pVol& volume);
        virtual bool raycast(const Ray& ray, std::vector<RaycastReport>& reports, bool isTest) const;

        std::vector<pVol> _eqs;
    };




    // IMPLEMENTATION //
    inline EPointPosition Volume::isIn(double x, double y, double z) const
    {
        return isIn(glm::dvec3(x, y, z));
    }

    inline double Volume::signedDistance(double x, double y, double z) const
    {
        return signedDistance(glm::dvec3(x, y, z));
    }

    inline void VolumeOr::add(const pVol& volume)
    {
        _eqs.push_back(volume);
    }

    inline void VolumeAnd::add(const pVol& volume)
    {
        _eqs.push_back(volume);
    }
}

#endif // PROPROOM3D_VOLUME_H
