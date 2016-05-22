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
                   const glm::dvec3& texV);

    public:
        glm::dvec3 texOrigin() const;
        glm::dvec3 texU() const;
        glm::dvec3 texV() const;
        glm::dvec3 texW() const;
        double texU2() const;
        double texV2() const;
        double texW2() const;

    protected:
        virtual void transform(const Transform& transform) override;


    private:
        glm::dvec3 _texOrigin;
        glm::dvec3 _texU, _texV, _texW;
        double _texU2, _texV2, _texW2;
    };

    class PROP3D_EXPORT BoxSideTexture : public BoxTexture
    {
    protected:
        BoxSideTexture(const glm::dvec3& minCorner,
                   const glm::dvec3& maxCorner,
                   const glm::dvec3& texOrigin,
                   const glm::dvec3& texU,
                   const glm::dvec3& texV);

    public:
        static std::shared_ptr<Surface>
            boxCorners(const glm::dvec3& minCorner,
                       const glm::dvec3& maxCorner,
                       const glm::dvec3& texOrigin,
                       const glm::dvec3& texU,
                       const glm::dvec3& texV);

        static std::shared_ptr<Surface>
            boxPosDims(const glm::dvec3& center,
                       const glm::dvec3& dimensions,
                       const glm::dvec3& texOrigin,
                       const glm::dvec3& texU,
                       const glm::dvec3& texV);

        // Node interface
        virtual void accept(Visitor& visitor) override;

        virtual void raycast(const Raycast& ray, RayHitList& reports) const override;
    };


    class PROP3D_EXPORT BoxBandTexture : public BoxTexture
    {
    protected:
        BoxBandTexture(const glm::dvec3& minCorner,
                   const glm::dvec3& maxCorner,
                   const glm::dvec3& texOrigin,
                   const glm::dvec3& texU,
                   const glm::dvec3& texV);

    public:
        static std::shared_ptr<Surface>
            boxCorners(const glm::dvec3& minCorner,
                       const glm::dvec3& maxCorner,
                       const glm::dvec3& texOrigin,
                       const glm::dvec3& texU,
                       const glm::dvec3& texV);

        static std::shared_ptr<Surface>
            boxPosDims(const glm::dvec3& center,
                       const glm::dvec3& dimensions,
                       const glm::dvec3& texOrigin,
                       const glm::dvec3& texU,
                       const glm::dvec3& texV);

        // Node interface
        virtual void accept(Visitor& visitor) override;

        virtual void raycast(const Raycast& ray, RayHitList& reports) const override;
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

    inline glm::dvec3 BoxTexture::texW() const
    {
        return _texW;
    }

    inline double BoxTexture::texU2() const
    {
        return _texU2;
    }

    inline double BoxTexture::texV2() const
    {
        return _texV2;
    }

    inline double BoxTexture::texW2() const
    {
        return _texW2;
    }
}

#endif // PROPROOM3D_BOX_H
