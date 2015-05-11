#ifndef PROPROOM2D_STDDESIGNER_H
#define PROPROOM2D_STDDESIGNER_H

#include "AbstractDesigner.h"


namespace prop2
{
    class PROP2D_EXPORT StdDesigner : public AbstractDesigner
    {
    public:
        StdDesigner();
        virtual ~StdDesigner();

        virtual void setup();
        virtual void reset();

        virtual std::shared_ptr<Circle>   createCircle();
        virtual std::shared_ptr<Polygon>  createPolygon(int nbVertices);
        virtual std::shared_ptr<TextHud>  createTextHud();
        virtual std::shared_ptr<ImageHud> createImageHud();
    };
}

#endif // PROPROOM2D_STDPROPDESIGNER_H
