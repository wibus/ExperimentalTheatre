#ifndef PROPROOM3D_BOX_H
#define PROPROOM3D_BOX_H

#include "Surface.h"


namespace prop3
{
    class PROP3D_EXPORT Box : public PhysicalSurface
    {
    protected:
        Box(const glm::dvec3& minCorner, const glm::dvec3& maxCorner);

    public:
        static std::shared_ptr<Surface>
            boxCorners(const glm::dvec3& minCorner, const glm::dvec3& maxCorner);
        static std::shared_ptr<Surface>
            boxPosDims(const glm::dvec3& center, const glm::dvec3& dimensions);

        // Node interface
        virtual void accept(Visitor& visitor) override;

        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;

        glm::dvec3 minCorner() const;

        glm::dvec3 maxCorner() const;

        virtual bool isAffineTransformable() const override {return false;}
        virtual bool isTranslatable() const override {return true;}
        virtual bool isRotatable() const override {return false;}
        virtual bool isScalable() const override {return true;}

    protected:
        virtual void transform(const Transform& transform) override;

    private:
        glm::dvec3 _center;
        glm::dvec3 _minCorner;
        glm::dvec3 _maxCorner;
        glm::dvec3 _dimensions;
        glm::dvec3 _invDim;
    };


    class PROP3D_EXPORT BoxTexture : public Box
    {
    protected:
        BoxTexture(const glm::dvec3& minCorner,
                   const glm::dvec3& maxCorner,
                   const glm::dvec3& texOrigin,
                   const glm::dvec3& texU,
                   const glm::dvec3& texV,
                   bool texMainSideOnly);

    public:
        static std::shared_ptr<Surface>
            boxCorners(const glm::dvec3& minCorner,
                       const glm::dvec3& maxCorner,
                       const glm::dvec3& texOrigin,
                       const glm::dvec3& texU,
                       const glm::dvec3& texV,
                       bool texMainSideOnly);

        static std::shared_ptr<Surface>
            boxPosDims(const glm::dvec3& center,
                       const glm::dvec3& dimensions,
                       const glm::dvec3& texOrigin,
                       const glm::dvec3& texU,
                       const glm::dvec3& texV,
                       bool texMainSideOnly);

        // Node interface
        virtual void accept(Visitor& visitor) override;

        virtual void raycast(const Raycast& ray, RayHitList& reports) const override;


        glm::dvec3 texOrigin() const;

        glm::dvec3 texU() const;

        glm::dvec3 texV() const;

        bool texMainSideOnly() const;

    protected:
        virtual void transform(const Transform& transform) override;

    private:
        glm::dvec3 _texOrigin;
        glm::dvec3 _texU, _texV;
        bool _texMainSideOnly;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 Box::minCorner() const
    {
        return _minCorner;
    }

    inline glm::dvec3 Box::maxCorner() const
    {
        return _maxCorner;
    }

    inline glm::dvec3 BoxTexture::texOrigin() const
    {
        return _texOrigin;
    }

    inline glm::dvec3 BoxTexture::texU() const
    {
        return _texU;
    }

    inline glm::dvec3 BoxTexture::texV() const
    {
        return _texV;
    }

    inline bool BoxTexture::texMainSideOnly() const
    {
        return _texMainSideOnly;
    }
}

#endif // PROPROOM3D_BOX_H
