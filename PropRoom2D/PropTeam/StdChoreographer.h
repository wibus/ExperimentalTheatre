#ifndef PROPROOM2D_STDCHOREOGRAPHER_H
#define PROPROOM2D_STDCHOREOGRAPHER_H

#include <vector>

#include "AbstractChoreographer.h"


namespace prop2
{
    class AbstractShape;

    class StdCollisionReport
    {
    public:
        StdCollisionReport() :
            areColliding(false),
            contactPoint(Vec2r(real(0.0), real(0.0))),
            contactNormal(Vec2r(real(1.0), real(0.0))),
            penetrationDepth(real(0.0)),
            shape1(nullptr),
            shape2(nullptr)
        {}

        bool areColliding;
        Vec2r contactPoint;
        Vec2r contactNormal;
        real penetrationDepth;
        std::shared_ptr<AbstractShape> shape1;
        std::shared_ptr<AbstractShape> shape2;
    };

    class PROP2D_EXPORT StdChoreographer : public AbstractChoreographer
    {
    public:
        StdChoreographer();
        virtual ~StdChoreographer();

        virtual void setGravity(const Vec2r& unitsPerSecondSquared);

        virtual void setup();
        virtual void reset();
        virtual void update(real dt);

        virtual void manageCircle(const std::shared_ptr<Circle>& circle);
        virtual void managePolygon(const std::shared_ptr<Polygon>& polygon);
        virtual void manageTextHud(const std::shared_ptr<TextHud>& text);
        virtual void manageImageHud(const std::shared_ptr<ImageHud>& image);

        virtual void unmanageCircle(const std::shared_ptr<Circle>& circle);
        virtual void unmanagePolygon(const std::shared_ptr<Polygon>& polygon);
        virtual void unmanageTextHud(const std::shared_ptr<TextHud>& text);
        virtual void unmanageImageHud(const std::shared_ptr<ImageHud>& image);

    protected:
        virtual void updateShape(
            const std::shared_ptr<AbstractShape>& shape);
        virtual void moveApart(
            const std::shared_ptr<StdCollisionReport>& report);
        virtual void resolveCollision(
            const std::shared_ptr<StdCollisionReport>& report);

        virtual std::shared_ptr<StdCollisionReport> detectCircleCircle(
            const std::shared_ptr<Circle>& circle1,
            const std::shared_ptr<Circle>& circle2);

        virtual std::shared_ptr<StdCollisionReport> detectCirclePolygon(
            const std::shared_ptr<Circle>& circle,
            const std::shared_ptr<Polygon>& polygon);

        virtual std::shared_ptr<StdCollisionReport> detectPolygonPolygon(
            const std::shared_ptr<Polygon>& polygon1,
            const std::shared_ptr<Polygon>& polygon2);

        real _dt;
        Vec2r _gravity;
        std::vector< std::shared_ptr<Circle> >  _circles;
        std::vector< std::shared_ptr<Polygon> > _polygons;

        real _maxHandledDeltaTime;
        real _correctionPercentage;
        real _correctionSlop;
    };



    // IMPLEMENTATION //
    inline void StdChoreographer::setGravity(const Vec2r& unitsPerSecondSquared)
    {
        _gravity = unitsPerSecondSquared;
    }
}

#endif // PROPROOM2D_STDCHOREOGRAPHER_H
