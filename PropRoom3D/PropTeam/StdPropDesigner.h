#ifndef PROPROOM3D_STDPROPDESIGNER_H
#define PROPROOM3D_STDPROPDESIGNER_H

#include "libPropRoom3D_global.h"
#include "AbstractPropDesigner.h"


namespace prop3
{
    class PROP3D_EXPORT StdPropDesigner : public AbstractPropDesigner
    {
    public:
        StdPropDesigner();
        virtual ~StdPropDesigner();

        virtual void setup();
        virtual void reset();

        virtual std::shared_ptr<Brep>   createBrep();
        virtual std::shared_ptr<Mesh>   createMesh(int triangleCount);
        virtual std::shared_ptr<Plane>  createPlane();
        virtual std::shared_ptr<Sphere> createSphere();
    };
}

#endif // PROPROOM3D_STDPROPDESIGNER_H
