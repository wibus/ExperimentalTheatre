#ifndef PROPROOM3D_BOX_H
#define PROPROOM3D_BOX_H

#include "Surface.h"


namespace prop3
{
    class PROP3D_EXPORT Box : public PhysicalSurface
    {
        Box(const glm::dvec3& minCorner, const glm::dvec3& maxCorner);

    public:
        static std::shared_ptr<Surface>
            boxCorners(const glm::dvec3& minCorner, const glm::dvec3& maxCorner);
        static std::shared_ptr<Surface>
            boxPosDims(const glm::dvec3& center, const glm::dvec3& dimensions);

        // StageSetNode interface
        virtual void accept(StageSetVisitor& visitor) override;

        virtual void transform(const Transform& transform);
        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Raycast& ray, RayHitList& reports) const;
        virtual bool intersects(const Raycast& ray, RayHitList& reports) const;


        glm::dvec3 minCorner() const;

        glm::dvec3 maxCorner() const;


    private:
        glm::dvec3 _center;
        glm::dvec3 _dimensions;
        glm::dvec3 _minCorner;
        glm::dvec3 _maxCorner;
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
}

#endif // PROPROOM3D_BOX_H
