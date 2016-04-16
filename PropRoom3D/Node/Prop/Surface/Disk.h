#ifndef PROPROOM3D_DISK_H
#define PROPROOM3D_DISK_H

#include "Surface.h"


namespace prop3
{
    class PROP3D_EXPORT Disk : public PhysicalSurface
    {
    protected:
        Disk(const glm::dvec3& center,
             const glm::dvec3& normal,
             double radius);

    public:
        static std::shared_ptr<Surface>
            disk(const glm::dvec3& center,
                 const glm::dvec3& normal,
                 double radius);

        // Node interface
        virtual void accept(Visitor& visitor) override;

        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;


        glm::dvec3 center() const;

        glm::dvec3 normal() const;

        double radius() const;


        virtual bool isAffineTransformable() const override {return true;}
        virtual bool isTranslatable() const override {return true;}
        virtual bool isRotatable() const override {return true;}
        virtual bool isScalable() const override {return true;}

    protected:
        virtual void transform(const Transform& transform) override;


    private:
        glm::dvec3 _center;
        glm::dvec3 _normal;
        double _radius;
        double _radius2;
        double _d;

        glm::dvec3 _transformN;

        glm::dmat4 _invTransform;
        glm::dmat4 _transform;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 Disk::center() const
    {
        return _center;
    }

    inline glm::dvec3 Disk::normal() const
    {
        return _normal;
    }

    inline double Disk::radius() const
    {
        return _radius;
    }
}

#endif // PROPROOM3D_DISK_H
