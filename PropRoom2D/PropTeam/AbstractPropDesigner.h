#ifndef PROPROOM2D_ABSTRACTPROPDESIGNER_H
#define PROPROOM2D_ABSTRACTPROPDESIGNER_H

#include "libPropRoom2D_global.h"
#include "AbstractTeamMember.h"

#include <memory>


namespace prop2
{
class Circle;
class Polygon;
class TextHud;
class ImageHud;

    class PROP2D_EXPORT AbstractPropDesigner : public AbstractTeamMember
    {
    protected:
        AbstractPropDesigner() {}

    public:
        virtual ~AbstractPropDesigner() {}

        virtual std::shared_ptr<Circle>   createCircle() =0;
        virtual std::shared_ptr<Polygon>  createPolygon(int nbVertices) =0;
        virtual std::shared_ptr<TextHud>  createTextHud() =0;
        virtual std::shared_ptr<ImageHud> createImageHud() =0;
    };
}

#endif // PROPROOM2D_ABSTRACTPROPDESIGNER_H
