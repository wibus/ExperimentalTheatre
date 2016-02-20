#ifndef PROPROOM3D_SURFACE_H
#define PROPROOM3D_SURFACE_H

#include <memory>
#include <vector>

#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

#include <PropRoom3D/Node/Node.h>


namespace prop3
{
    class Raycast;
    class RayHitList;
    class Coating;
    class Material;


    enum class EPointPosition
    {
        OUT,
        ON,
        IN
    };


    struct PROP3D_EXPORT Transform
    {
        Transform(const glm::dmat4& mat);

        const glm::dmat4& mat() const { return _mat; }
        const glm::dmat4& inv() const;

    private:
        glm::dmat4 _mat;
        mutable glm::dmat4 _inv;
        mutable bool _isInvComputed;
    };



    class PROP3D_EXPORT Surface : public Node
    {
        friend void applyTransformation(
                const std::shared_ptr<Surface>& surf,
                const Transform& transform);

    protected:
        Surface();

    public:
        virtual ~Surface();

        EPointPosition isIn(double x, double y, double z) const;
        virtual EPointPosition isIn(const glm::dvec3& point) const = 0;

        double signedDistance(double x, double y, double z) const;
        virtual double signedDistance(const glm::dvec3& point) const = 0;

        virtual void raycast(const Raycast& ray, RayHitList& reports) const = 0;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const = 0;

        virtual void setCoating(const std::shared_ptr<Coating>& coating) = 0;
        virtual void setInnerMaterial(const std::shared_ptr<Material>& mat) = 0;
        virtual void setOuterMaterial(const std::shared_ptr<Material>& mat) = 0;

        virtual bool isAffineTransformable() const {return false;}
        virtual bool isTranslatable() const {return false;}
        virtual bool isRotatable() const {return false;}
        virtual bool isScalable() const {return false;}

        // Transform tools
        static std::shared_ptr<Surface> shell(const std::shared_ptr<Surface>& surf);
        static std::shared_ptr<Surface> transform(std::shared_ptr<Surface>& surf, const glm::dmat4& mat);
        static std::shared_ptr<Surface> translate(std::shared_ptr<Surface>& surf, const glm::dvec3& dis);
        static std::shared_ptr<Surface> rotate(std::shared_ptr<Surface>& surf, double angle, const glm::dvec3& axis);
        static std::shared_ptr<Surface> scale(std::shared_ptr<Surface>& surf, double coeff);

        // Default coatings and materials
        static const std::shared_ptr<Coating>  NO_COATING;
        static const std::shared_ptr<Material> DEFAULT_MATERIAL;
        static const std::shared_ptr<Material> ENVIRONMENT_MATERIAL;


    protected:
        virtual void transform(const Transform& transform) = 0;
    };


    // Physical surfaces
    class PROP3D_EXPORT PhysicalSurface : public Surface
    {
    protected:
        PhysicalSurface();

    public:
        virtual std::vector<std::shared_ptr<Node>> children() const override;

        virtual void setCoating(const std::shared_ptr<Coating>& coating) override;
        virtual void setInnerMaterial(const std::shared_ptr<Material>& mat) override;
        virtual void setOuterMaterial(const std::shared_ptr<Material>& mat) override;

        std::shared_ptr<Coating> coating() const;
        std::shared_ptr<Material> innerMaterial() const;
        std::shared_ptr<Material> outerMaterial() const;

    protected:
        std::shared_ptr<Coating>  _coating;
        std::shared_ptr<Material> _innerMat;
        std::shared_ptr<Material> _outerMat;
    };


    // Surface shell protects surfaces by intercepting coating assignation
    // and transformation application by caching those internally
    class PROP3D_EXPORT SurfaceShell : public PhysicalSurface
    {
    protected:
        friend PROP3D_EXPORT std::shared_ptr<Surface> Surface::shell(
                const std::shared_ptr<Surface> &surf);
        SurfaceShell(const std::shared_ptr<Surface>& surf);

    public:
        // Node interface
        virtual void accept(Visitor& visitor) override;
        virtual std::vector<std::shared_ptr<Node>> children() const override;

        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;

        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;

        virtual bool isAffineTransformable() const override {return true;}
        virtual bool isTranslatable() const override {return true;}
        virtual bool isRotatable() const override {return true;}
        virtual bool isScalable() const override {return true;}

        glm::dmat4 transform() const;

    protected:
        virtual void transform(const Transform& transform) override;

    private:
        std::shared_ptr<Surface> _surf;
        glm::dmat4 _invTransform;
        glm::dmat4 _mvTransform;
        glm::dmat3 _normalTransform;
    };


    // Logical surfaces
    class PROP3D_EXPORT SurfaceGhost : public Surface
    {
        friend PROP3D_EXPORT std::shared_ptr<Surface> operator~ (
                const std::shared_ptr<Surface>&);
        SurfaceGhost(const std::shared_ptr<Surface>& surf);

    public:
        // Node interface
        virtual void accept(Visitor& visitor) override;
        virtual std::vector<std::shared_ptr<Node>> children() const override;

        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;

        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;

        virtual void setCoating(const std::shared_ptr<Coating>& coating);
        virtual void setInnerMaterial(const std::shared_ptr<Material>& mat) override;
        virtual void setOuterMaterial(const std::shared_ptr<Material>& mat) override;

        virtual bool isAffineTransformable() const override;
        virtual bool isTranslatable() const override;
        virtual bool isRotatable() const override;
        virtual bool isScalable() const override;

    protected:
        virtual void transform(const Transform& transform) override;

    private:
        std::shared_ptr<Surface> _surf;
    };


    class PROP3D_EXPORT SurfaceInverse : public Surface
    {
        friend PROP3D_EXPORT std::shared_ptr<Surface> operator! (
                const std::shared_ptr<Surface>&);
        SurfaceInverse(const std::shared_ptr<Surface>& surf);

    public:
        // Node interface
        virtual void accept(Visitor& visitor) override;
        virtual std::vector<std::shared_ptr<Node>> children() const override;

        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;

        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;

        virtual void setCoating(const std::shared_ptr<Coating>& coating);
        virtual void setInnerMaterial(const std::shared_ptr<Material>& mat) override;
        virtual void setOuterMaterial(const std::shared_ptr<Material>& mat) override;

        virtual bool isAffineTransformable() const override;
        virtual bool isTranslatable() const override;
        virtual bool isRotatable() const override;
        virtual bool isScalable() const override;

    protected:
        virtual void transform(const Transform& transform) override;

    private:
        std::shared_ptr<Surface> _surf;
    };


    class PROP3D_EXPORT SurfaceOr : public Surface
    {
        friend PROP3D_EXPORT std::shared_ptr<Surface> operator| (
                const std::shared_ptr<Surface>&,
                const std::shared_ptr<Surface>&);
        SurfaceOr(const std::vector<std::shared_ptr<Surface>>& surfs);

    public:
        static std::shared_ptr<Surface> apply(
                const std::vector<std::shared_ptr<Surface>>& surfs);

        virtual void accept(Visitor& visitor) override;
        virtual std::vector<std::shared_ptr<Node>> children() const override;

        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;

        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;

        virtual void setCoating(const std::shared_ptr<Coating>& coating);
        virtual void setInnerMaterial(const std::shared_ptr<Material>& mat) override;
        virtual void setOuterMaterial(const std::shared_ptr<Material>& mat) override;

        virtual bool isAffineTransformable() const override;
        virtual bool isTranslatable() const override;
        virtual bool isRotatable() const override;
        virtual bool isScalable() const override;

    protected:
        virtual void transform(const Transform& transform) override;

    private:
        void add(const std::shared_ptr<Surface>& surface);

        std::vector<std::shared_ptr<Surface>> _surfs;
    };


    class PROP3D_EXPORT SurfaceAnd : public Surface
    {
        friend PROP3D_EXPORT std::shared_ptr<Surface> operator& (
                const std::shared_ptr<Surface>&,
                const std::shared_ptr<Surface>&);
        SurfaceAnd(const std::vector<std::shared_ptr<Surface>>& surfs);

    public:
        static std::shared_ptr<Surface> apply(
                const std::vector<std::shared_ptr<Surface>>& surfs);

        virtual void accept(Visitor& visitor) override;
        virtual std::vector<std::shared_ptr<Node>> children() const override;

        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;

        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;

        virtual void setCoating(const std::shared_ptr<Coating>& coating);
        virtual void setInnerMaterial(const std::shared_ptr<Material>& mat) override;
        virtual void setOuterMaterial(const std::shared_ptr<Material>& mat) override;

        virtual bool isAffineTransformable() const override;
        virtual bool isTranslatable() const override;
        virtual bool isRotatable() const override;
        virtual bool isScalable() const override;

    protected:
        virtual void transform(const Transform& transform) override;

    private:
        void add(const std::shared_ptr<Surface>& surface);

        std::vector<std::shared_ptr<Surface>> _surfs;
    };


    // Logical operators overloading
    // Ghost surface
	PROP3D_EXPORT std::shared_ptr<Surface> operator~ (
            const std::shared_ptr<Surface>& surf);
    // Invert sides
	PROP3D_EXPORT std::shared_ptr<Surface> operator! (
            const std::shared_ptr<Surface>& surf);
    // Surface union
	PROP3D_EXPORT std::shared_ptr<Surface> operator| (
            const std::shared_ptr<Surface>& surf1,
            const std::shared_ptr<Surface>& surf2);
    // Surface intersection
	PROP3D_EXPORT std::shared_ptr<Surface> operator& (
            const std::shared_ptr<Surface>& surf1,
            const std::shared_ptr<Surface>& surf2);
    // Surface junction
	PROP3D_EXPORT std::shared_ptr<Surface> operator^ (
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

    inline std::shared_ptr<Material> PhysicalSurface::innerMaterial() const
    {
        return _innerMat;
    }

    inline std::shared_ptr<Material> PhysicalSurface::outerMaterial() const
    {
        return _outerMat;
    }

    inline glm::dmat4 SurfaceShell::transform() const
    {
        return _mvTransform;
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
