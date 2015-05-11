#ifndef PROPROOM2D_ABSTRACTCHOREOGRAPHER_H
#define PROPROOM2D_ABSTRACTCHOREOGRAPHER_H

#include <memory>

#include <GLM/glm.hpp>

#include "../../libPropRoom2D_global.h"


namespace prop2
{
    class Circle;
    class Polygon;
    class TextHud;
    class ImageHud;

    class PROP2D_EXPORT AbstractChoreographer
    {
    protected:
        AbstractChoreographer() {}

    public:
        virtual ~AbstractChoreographer() {}

        virtual void setup() =0;
        virtual void reset() =0;
        virtual void update(double dt) =0;

        virtual void setGravity(const glm::dvec2& unitsPerSecondSquared) =0;

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
