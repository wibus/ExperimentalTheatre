#ifndef PROPROOM2D_STDCHOREOGRAPHER_H
#define PROPROOM2D_STDCHOREOGRAPHER_H

#include "libPropRoom2D_global.h"
#include "AbstractChoreographer.h"

#include <vector>


namespace prop2
{
class AbstractShape;

    class PROP2D_EXPORT StdChoreographer : public AbstractChoreographer
    {
    public:
        StdChoreographer();
        virtual ~StdChoreographer();

        virtual void setGravity(real yMeterPerSeconds2);

        virtual void setup();
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
        virtual void updateVelocity(AbstractShape* shape);
        virtual void updatePosition(AbstractShape* shape);

        virtual void resolveCircleCircle(
            std::shared_ptr<Circle>& circle1,
            std::shared_ptr<Circle>& circle2);

        virtual void resolveCirclePolygon(
            std::shared_ptr<Circle>& circle,
            std::shared_ptr<Polygon>& polygon);

        virtual void resolvePolygonPolygon(
            std::shared_ptr<Polygon>& polygon1,
            std::shared_ptr<Polygon>& polygon2);

        real _dt;
        real _gravity;
        std::vector< std::shared_ptr<Circle> >  _circles;
        std::vector< std::shared_ptr<Polygon> > _polygons;
    };



    // IMPLEMENTATION //
    inline void StdChoreographer::setGravity(real yMeterPerSeconds2)
    {
        _gravity = yMeterPerSeconds2;
    }
}

#endif // PROPROOM2D_STDCHOREOGRAPHER_H
