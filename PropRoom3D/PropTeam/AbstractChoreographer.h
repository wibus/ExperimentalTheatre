#ifndef PROPROOM3D_ABSTRACTCHOREOGRAPHER_H
#define PROPROOM3D_ABSTRACTCHOREOGRAPHER_H

#include "libPropRoom3D_global.h"
#include "AbstractTeamMember.h"

#include <memory>

#include <GLM/glm.hpp>


namespace prop3
{
    class PROP3D_EXPORT AbstractChoreographer : public AbstractTeamMember
    {
    protected:
        AbstractChoreographer() {}

    public:
        virtual ~AbstractChoreographer() {}

        virtual void setGravity(const glm::dvec3& unitsPerSecondSquared) =0;

        virtual void update(double dt) =0;

        virtual void manageBrep(const std::shared_ptr<Brep>& brep) =0;
        virtual void manageMesh(const std::shared_ptr<Mesh>& mesh) =0;

        virtual void unmanageBrep(const std::shared_ptr<Brep>& brep) =0;
        virtual void unmanageMesh(const std::shared_ptr<Mesh>& mesh) =0;
    };
}

#endif // PROPROOM3D_ABSTRACTCHOREOGRAPHER_H
