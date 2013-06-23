#ifndef PROPROOM2D_STDPROPDESIGNER_H
#define PROPROOM2D_STDPROPDESIGNER_H

#include "libPropRoom2D_global.h"
#include "AbstractPropDesigner.h"


namespace prop2
{
    class PROP2D_EXPORT StdPropDesigner : public AbstractPropDesigner
    {
    public:
        StdPropDesigner();
        virtual ~StdPropDesigner();

        virtual void setup();
        virtual void reset();

        virtual std::shared_ptr<Circle>   createCircle();
        virtual std::shared_ptr<Polygon>  createPolygon(int nbVertices);
        virtual std::shared_ptr<TextHud>  createTextHud();
        virtual std::shared_ptr<ImageHud> createImageHud();
    };
}

#endif // PROPROOM2D_STDPROPDESIGNER_H
