#ifndef PROPROOM3D_IMPLICITSURFACE_H
#define PROPROOM3D_IMPLICITSURFACE_H

#include <memory>
#include <vector>

#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    class Ray;
    class RayHitReport;
    class Coating;


    enum class EPointPosition
    {
        OUT,
        ON,
        IN
    };


    struct PROP3D_EXPORT Transform
    {
        Transform(const glm::dmat4& mat);
        Transform(double scale ,
                  const glm::dquat& rotation,
                  const glm::dvec3& translation);

        const glm::dmat4& mat() const { return _mat; }
        const glm::dmat4& inv() const;

    private:
        glm::dmat4 _mat;
        mutable glm::dmat4 _inv;
        mutable bool _isInvComputed;
    };



    class PROP3D_EXPORT ImplicitSurface
    {
    protected:
        ImplicitSurface();

    public:
        virtual ~ImplicitSurface();

        virtual void transform(const Transform& transform) = 0;

        EPointPosition isIn(double x, double y, double z) const;
        virtual EPointPosition isIn(const glm::dvec3& point) const = 0;

        double signedDistance(double x, double y, double z) const;
        virtual double signedDistance(const glm::dvec3& point) const = 0;

        virtual void raycast(const Ray& ray, std::vector<RayHitReport>& reports) const =0;
        virtual bool intersects(const Ray& ray) = 0;

        virtual void setCoating(const std::shared_ptr<Coating>& coating) = 0;

    protected:
        static const std::shared_ptr<Coating> NO_COATING;
    };


    // Logical surfaces
    class PROP3D_EXPORT SurfaceGhost : public ImplicitSurface
    {
        friend std::shared_ptr<ImplicitSurface> operator~ (
                const std::shared_ptr<ImplicitSurface>&);
        SurfaceGhost(const std::shared_ptr<ImplicitSurface>& eq);

    public:
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RayHitReport>& reports) const;
        virtual bool intersects(const Ray& ray);
        virtual void setCoating(const std::shared_ptr<Coating>& coating);

    private:
        std::shared_ptr<ImplicitSurface> _eq;
    };


    class PROP3D_EXPORT SurfaceInverse : public ImplicitSurface
    {
        friend std::shared_ptr<ImplicitSurface> operator! (
                const std::shared_ptr<ImplicitSurface>&);
        SurfaceInverse(const std::shared_ptr<ImplicitSurface>& eq);

    public:
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RayHitReport>& reports) const;
        virtual bool intersects(const Ray& ray);
        virtual void setCoating(const std::shared_ptr<Coating>& coating);

    private:
        std::shared_ptr<ImplicitSurface> _eq;
    };


    class PROP3D_EXPORT SurfaceOr : public ImplicitSurface
    {
        friend std::shared_ptr<ImplicitSurface> operator| (
                const std::shared_ptr<ImplicitSurface>&,
                const std::shared_ptr<ImplicitSurface>&);
        SurfaceOr(const std::vector<std::shared_ptr<ImplicitSurface>>& eqs);

    public:
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RayHitReport>& reports) const;
        virtual bool intersects(const Ray& ray);
        virtual void setCoating(const std::shared_ptr<Coating>& coating);

    private:
        void add(const std::shared_ptr<ImplicitSurface>& surface);
        virtual bool raycast(const Ray& ray, std::vector<RayHitReport>& reports, bool isTest) const;

        std::vector<std::shared_ptr<ImplicitSurface>> _eqs;
    };


    class PROP3D_EXPORT SurfaceAnd : public ImplicitSurface
    {
        friend std::shared_ptr<ImplicitSurface> operator& (
                const std::shared_ptr<ImplicitSurface>&,
                const std::shared_ptr<ImplicitSurface>&);
        SurfaceAnd(const std::vector<std::shared_ptr<ImplicitSurface>>& eqs);

    public:
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RayHitReport>& reports) const;
        virtual bool intersects(const Ray& ray);
        virtual void setCoating(const std::shared_ptr<Coating>& coating);

    private:
        void add(const std::shared_ptr<ImplicitSurface>& surface);
        virtual bool raycast(const Ray& ray, std::vector<RayHitReport>& reports, bool isTest) const;

        std::vector<std::shared_ptr<ImplicitSurface>> _eqs;
    };


    // Logical operators overloading
    // Ghost surface
    std::shared_ptr<ImplicitSurface> operator~ (
            const std::shared_ptr<ImplicitSurface>& eq);
    // Invert sides
    std::shared_ptr<ImplicitSurface> operator! (
            const std::shared_ptr<ImplicitSurface>& eq);
    // Surface union
    std::shared_ptr<ImplicitSurface> operator| (
            const std::shared_ptr<ImplicitSurface>& eq1,
            const std::shared_ptr<ImplicitSurface>& eq2);
    // Surface intersection
    std::shared_ptr<ImplicitSurface> operator& (
            const std::shared_ptr<ImplicitSurface>& eq1,
            const std::shared_ptr<ImplicitSurface>& eq2);




    // IMPLEMENTATION //
    inline EPointPosition ImplicitSurface::isIn(double x, double y, double z) const
    {
        return isIn(glm::dvec3(x, y, z));
    }

    inline double ImplicitSurface::signedDistance(double x, double y, double z) const
    {
        return signedDistance(glm::dvec3(x, y, z));
    }

    inline void SurfaceOr::add(const std::shared_ptr<ImplicitSurface>& surface)
    {
        _eqs.push_back(surface);
    }

    inline void SurfaceAnd::add(const std::shared_ptr<ImplicitSurface>& surface)
    {
        _eqs.push_back(surface);
    }
}

#endif // PROPROOM3D_IMPLICITSURFACE_H
