#ifndef PROPROOM3D_QGLPOSTPRODUNIT_H
#define PROPROOM3D_QGLPOSTPRODUNIT_H

#include <MediaWorkbench/GL/GlProgram.h>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    class PROP3D_EXPORT QGlPostProdUnit
    {
    public:
        QGlPostProdUnit();
        virtual ~QGlPostProdUnit();

        virtual void setColorBufferTexId(unsigned int id);

        virtual void setup();
        virtual void execute();

    private:
        unsigned int _colorBufferTexId;

        media::GlProgram _postProdProgram;
        unsigned int _fullscreenVao;
        unsigned int _fullscreenVbo;
    };
}

#endif // PROPROOM3D_QGLPOSTPRODUNIT_H
