#ifndef PROPROOM3D_ABSTRACTPROPDESIGNER_H
#define PROPROOM3D_ABSTRACTPROPDESIGNER_H

#include "libPropRoom3D_global.h"
#include "AbstractTeamMember.h"

#include <memory>


namespace prop3
{
    class PROP3D_EXPORT AbstractPropDesigner : public AbstractTeamMember
    {
    protected:
        AbstractPropDesigner() {}

    public:
        virtual ~AbstractPropDesigner() {}

        virtual std::shared_ptr<Brep>   createBrep() =0;
        virtual std::shared_ptr<Mesh>   createMesh(int triangleCount) =0;
        virtual std::shared_ptr<Plane>  createPlane() =0;
        virtual std::shared_ptr<Sphere> createSphere() =0;
    };
}

#endif // PROPROOM3D_ABSTRACTPROPDESIGNER_H
