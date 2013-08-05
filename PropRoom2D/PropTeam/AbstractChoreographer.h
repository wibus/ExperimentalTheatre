#ifndef PROPROOM2D_ABSTRACTCHOREOGRAPHER_H
#define PROPROOM2D_ABSTRACTCHOREOGRAPHER_H

#include "libPropRoom2D_global.h"
#include "AbstractTeamMember.h"

#include <memory>


namespace prop2
{
class Circle;
class Polygon;
class TextHud;
class ImageHud;

    class PROP2D_EXPORT AbstractChoreographer : public AbstractTeamMember
    {
    protected:
        AbstractChoreographer() {}

    public:
        virtual ~AbstractChoreographer() {}

        virtual void setGravity(const Vec2r& unitsPerSecondSquared) =0;

        virtual void update(real dt) =0;

        virtual void manageCircle(const std::shared_ptr<Circle>& circle) =0;
        virtual void managePolygon(const std::shared_ptr<Polygon>& polygon) =0;
        virtual void manageTextHud(const std::shared_ptr<TextHud>& text) =0;
        virtual void manageImageHud(const std::shared_ptr<ImageHud>& image) =0;

        virtual void unmanageCircle(const std::shared_ptr<Circle>& circle) =0;
        virtual void unmanagePolygon(const std::shared_ptr<Polygon>& polygon) =0;
        virtual void unmanageTextHud(const std::shared_ptr<TextHud>& text) =0;
        virtual void unmanageImageHud(const std::shared_ptr<ImageHud>& image) =0;
    };
}

#endif // PROPROOM2D_ABSTRACTCHOREOGRAPHER_H
