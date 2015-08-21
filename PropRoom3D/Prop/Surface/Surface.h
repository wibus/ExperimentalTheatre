#ifndef PROPROOM3D_SURFACE_H
#define PROPROOM3D_SURFACE_H

#include <memory>
#include <vector>

#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

#include "PropRoom3D/Scene/SceneNode.h"


namespace prop3
{
    class Ray;
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



    class PROP3D_EXPORT Surface : public SceneNode
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

        virtual void raycast(const Ray& ray, RayHitList& reports) const =0;
        virtual bool intersects(const Ray& ray, RayHitList& reports) const = 0;

        virtual void setCoating(const std::shared_ptr<Coating>& coating) = 0;

    protected:
        static const std::shared_ptr<Coating> NO_COATING;
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
        virtual void raycast(const Ray& ray, RayHitList& reports) const;
        virtual bool intersects(const Ray& ray, RayHitList& reports) const;
        virtual void setCoating(const std::shared_ptr<Coating>& coating);

        // SceneNode interface
        virtual void accept(SceneVisitor& visitor) override;

        virtual std::vector<std::shared_ptr<SceneNode>> children() const override;


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
        virtual void raycast(const Ray& ray, RayHitList& reports) const;
        virtual bool intersects(const Ray& ray, RayHitList& reports) const;
        virtual void setCoating(const std::shared_ptr<Coating>& coating);

        // SceneNode interface
        virtual void accept(SceneVisitor& visitor) override;

        virtual std::vector<std::shared_ptr<SceneNode>> children() const override;


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
        virtual void raycast(const Ray& ray, RayHitList& reports) const;
        virtual bool intersects(const Ray& ray, RayHitList& reports) const;
        virtual void setCoating(const std::shared_ptr<Coating>& coating);

        // SceneNode interface
        virtual void accept(SceneVisitor& visitor) override;

        virtual std::vector<std::shared_ptr<SceneNode>> children() const override;


    private:
        void add(const std::shared_ptr<Surface>& surface);
        virtual bool raycast(const Ray& ray, RayHitList& reports, bool isTest) const;

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
        virtual void raycast(const Ray& ray, RayHitList& reports) const;
        virtual bool intersects(const Ray& ray, RayHitList& reports) const;
        virtual void setCoating(const std::shared_ptr<Coating>& coating);

        // SceneNode interface
        virtual void accept(SceneVisitor& visitor) override;

        virtual std::vector<std::shared_ptr<SceneNode>> children() const override;


    private:
        void add(const std::shared_ptr<Surface>& surface);
        virtual bool raycast(const Ray& ray, RayHitList& reports, bool isTest) const;

        std::vector<std::shared_ptr<Surface>> _surfs;
    };


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
