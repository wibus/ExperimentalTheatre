#ifndef PROPROOM3D_ABSTRACTARTDIRECTOR_H
#define PROPROOM3D_ABSTRACTARTDIRECTOR_H

#include <memory>

#include <GLM/glm.hpp>

#include <CellarWorkbench/DesignPattern/SpecificObserver.h>

#include "../AbstractTeamMember.h"


namespace cellar
{
    class CameraMsg;
}

namespace prop3
{
    class PROP3D_EXPORT AbstractArtDirector :
            public cellar::SpecificObserver<cellar::CameraMsg>,
            public AbstractTeamMember
    {
    protected:
        AbstractArtDirector() {}

    public:
        virtual ~AbstractArtDirector() {}

        virtual void draw(double dt) =0;

        virtual void notify(cellar::CameraMsg &msg) = 0;

        virtual void manageProp(const std::shared_ptr<Prop>& prop) =0;
        virtual void unmanageProp(const std::shared_ptr<Prop>& prop) =0;
    };
}

#endif // PROPROOM3D_ABSTRACTARTDIRECTOR_H
