#ifndef PROPROOM3D_SURFACE_H
#define PROPROOM3D_SURFACE_H

#include <memory>
#include <vector>

#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

#include "PropRoom3D/StageSet/StageSetNode.h"


namespace prop3
{
    class Raycast;
    class RayHitList;
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
        Transform(double scale,
                  const glm::dquat& rotation,
                  const glm::dvec3& translation);

        const glm::dmat4& mat() const { return _mat; }
        const glm::dmat4& inv() const;

    private:
        glm::dmat4 _mat;
        mutable glm::dmat4 _inv;
        mutable bool _isInvComputed;
    };



    class PROP3D_EXPORT Surface : public StageSetNode
    {
    protected:
        Surface();

    public:
        virtual ~Surface();

        virtual void transform(const Transform& transform) = 0;

        EPointPosition isIn(double x, double y, double z) const;
        virtual EPointPosition isIn(const glm::dvec3& point) const = 0;

        double signedDistance(double x, double y, double z) const;
        virtual double signedDistance(const glm::dvec3& point) const = 0;

        virtual void raycast(const Raycast& ray, RayHitList& reports) const = 0;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const = 0;

        virtual void setCoating(const std::shared_ptr<Coating>& coating) = 0;


    protected:
        static const std::shared_ptr<Coating> NO_COATING;
    };


    // Physical surfaces
    class PROP3D_EXPORT PhysicalSurface : public Surface
    {
    protected:
        PhysicalSurface();

    public:
        virtual void setCoating(const std::shared_ptr<Coating>& coating) override;
        virtual std::vector<std::shared_ptr<StageSetNode>> children() const override;
        std::shared_ptr<Coating> coating() const;

    protected:
        std::shared_ptr<Coating>_coating;
    };


    // Surface shell protects surfaces by intercepting coating assignation
    // and transformation application by caching those internally
    class PROP3D_EXPORT SurfaceShell : public PhysicalSurface
    {
        friend std::shared_ptr<Surface> Shell(
                const std::shared_ptr<Surface>& surf);
        SurfaceShell(const std::shared_ptr<Surface>& surf);

    public:
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;

        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;

        virtual std::vector<std::shared_ptr<StageSetNode>> children() const override;

        glm::dmat4 transform() const;

    private:
        std::shared_ptr<Surface> _surf;
        glm::dmat4 _invTransform;
        glm::dmat4 _transform;
    };

    // Logical surfaces
    class PROP3D_EXPORT SurfaceGhost : public Surface
    {
        friend std::shared_ptr<Surface> operator~ (
                const std::shared_ptr<Surface>&);
        SurfaceGhost(const std::shared_ptr<Surface>& surf);

    public:
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;
        virtual void setCoating(const std::shared_ptr<Coating>& coating);

        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;

        virtual std::vector<std::shared_ptr<StageSetNode>> children() const override;


    private:
        std::shared_ptr<Surface> _surf;
    };


    class PROP3D_EXPORT SurfaceInverse : public Surface
    {
        friend std::shared_ptr<Surface> operator! (
                const std::shared_ptr<Surface>&);
        SurfaceInverse(const std::shared_ptr<Surface>& surf);

    public:
        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;
        virtual void setCoating(const std::shared_ptr<Coating>& coating);

        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;

        virtual std::vector<std::shared_ptr<StageSetNode>> children() const override;


    private:
        std::shared_ptr<Surface> _surf;
    };


    class PROP3D_EXPORT SurfaceOr : public Surface
    {
        friend std::shared_ptr<Surface> operator| (
                const std::shared_ptr<Surface>&,
                const std::shared_ptr<Surface>&);
        SurfaceOr(const std::vector<std::shared_ptr<Surface>>& surfs);

    public:
        static std::shared_ptr<Surface> apply(
                const std::vector<std::shared_ptr<Surface>>& surfs);

        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;
        virtual void setCoating(const std::shared_ptr<Coating>& coating);

        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;

        virtual std::vector<std::shared_ptr<StageSetNode>> children() const override;


    private:
        void add(const std::shared_ptr<Surface>& surface);
        virtual bool raycast(const Raycast& ray, RayHitList& reports, bool isTest) const;

        std::vector<std::shared_ptr<Surface>> _surfs;
    };


    class PROP3D_EXPORT SurfaceAnd : public Surface
    {
        friend std::shared_ptr<Surface> operator& (
                const std::shared_ptr<Surface>&,
                const std::shared_ptr<Surface>&);
        SurfaceAnd(const std::vector<std::shared_ptr<Surface>>& surfs);

    public:
        static std::shared_ptr<Surface> apply(
                const std::vector<std::shared_ptr<Surface>>& surfs);

        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;
        virtual void setCoating(const std::shared_ptr<Coating>& coating);

        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;

        virtual std::vector<std::shared_ptr<StageSetNode>> children() const override;


    private:
        void add(const std::shared_ptr<Surface>& surface);
        virtual bool raycast(const Raycast& ray, RayHitList& reports, bool isTest) const;

        std::vector<std::shared_ptr<Surface>> _surfs;
    };

    // Surface shell
    std::shared_ptr<Surface> Shell(
            const std::shared_ptr<Surface>& surf);

    // Logical operators overloading
    // Ghost surface
    std::shared_ptr<Surface> operator~ (
            const std::shared_ptr<Surface>& surf);
    // Invert sides
    std::shared_ptr<Surface> operator! (
            const std::shared_ptr<Surface>& surf);
    // Surface union
    std::shared_ptr<Surface> operator| (
            const std::shared_ptr<Surface>& surf1,
            const std::shared_ptr<Surface>& surf2);
    // Surface intersection
    std::shared_ptr<Surface> operator& (
            const std::shared_ptr<Surface>& surf1,
            const std::shared_ptr<Surface>& surf2);




    // IMPLEMENTATION //
    inline EPointPosition Surface::isIn(double x, double y, double z) const
    {
        return isIn(glm::dvec3(x, y, z));
    }

    inline double Surface::signedDistance(double x, double y, double z) const
    {
        return signedDistance(glm::dvec3(x, y, z));
    }

    inline std::shared_ptr<Coating> PhysicalSurface::coating() const
    {
        return _coating;
    }

    inline glm::dmat4 SurfaceShell::transform() const
    {
        return _transform;
    }

    inline std::shared_ptr<Surface> SurfaceOr::apply(
            const std::vector<std::shared_ptr<Surface>>& surfs)
    {
        return std::shared_ptr<SurfaceOr>(new SurfaceOr(surfs));
    }

    inline void SurfaceOr::add(const std::shared_ptr<Surface>& surface)
    {
        _surfs.push_back(surface);
    }

    inline std::shared_ptr<Surface> SurfaceAnd::apply(
            const std::vector<std::shared_ptr<Surface>>& surfs)
    {
        return std::shared_ptr<SurfaceAnd>(new SurfaceAnd(surfs));
    }

    inline void SurfaceAnd::add(const std::shared_ptr<Surface>& surface)
    {
        _surfs.push_back(surface);
    }
}

#endif // PROPROOM3D_SURFACE_H
