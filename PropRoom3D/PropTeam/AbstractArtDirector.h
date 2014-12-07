#ifndef PROPROOM3D_ABSTRACTARTDIRECTOR_H
#define PROPROOM3D_ABSTRACTARTDIRECTOR_H

#include "libPropRoom3D_global.h"
#include "AbstractTeamMember.h"

#include <memory>

#include <DesignPattern/SpecificObserver.h>


namespace media
{
class CameraMsg;
}

namespace prop3
{
    class PROP3D_EXPORT AbstractArtDirector :
            public cellar::SpecificObserver<media::CameraMsg>,
            public AbstractTeamMember
    {
    protected:
        AbstractArtDirector() {}

    public:
        virtual ~AbstractArtDirector() {}

        virtual void notify(media::CameraMsg &msg) = 0;

        virtual void draw(double dt) =0;

        virtual void manageBrep(const std::shared_ptr<Brep>& brep) =0;
        virtual void manageMesh(const std::shared_ptr<Mesh>& mesh) =0;
        virtual void managePlane(const std::shared_ptr<Plane>& plane) =0;
        virtual void manageSphere(const std::shared_ptr<Sphere>& sphere) =0;

        virtual void unmanageBrep(const std::shared_ptr<Brep>& brep) =0;
        virtual void unmanageMesh(const std::shared_ptr<Mesh>& mesh) =0;
        virtual void unmanagePlane(const std::shared_ptr<Plane>& plane) =0;
        virtual void unmanageSphere(const std::shared_ptr<Sphere>& sphere) =0;
    };
}

#endif // PROPROOM3D_ABSTRACTARTDIRECTOR_H
