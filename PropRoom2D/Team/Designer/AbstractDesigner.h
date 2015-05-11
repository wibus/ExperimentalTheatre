#ifndef PROPROOM2D_ABSTRACTDESIGNER_H
#define PROPROOM2D_ABSTRACTDESIGNER_H

#include <memory>

#include "../../libPropRoom2D_global.h"


namespace prop2
{
    class Circle;
    class Polygon;
    class TextHud;
    class ImageHud;

    class PROP2D_EXPORT AbstractDesigner
    {
    protected:
        AbstractDesigner() {}

    public:
        virtual ~AbstractDesigner() {}

        virtual void setup() =0;
        virtual void reset() =0;

        virtual std::shared_ptr<Circle>   createCircle() =0;
        virtual std::shared_ptr<Polygon>  createPolygon(int nbVertices) =0;
        virtual std::shared_ptr<TextHud>  createTextHud() =0;
        virtual std::shared_ptr<ImageHud> createImageHud() =0;
    };
}

#endif // PROPROOM2D_ABSTRACTDESIGNER_H
