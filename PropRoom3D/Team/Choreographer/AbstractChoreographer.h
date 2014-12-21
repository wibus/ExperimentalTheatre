#ifndef PROPROOM3D_ABSTRACTCHOREOGRAPHER_H
#define PROPROOM3D_ABSTRACTCHOREOGRAPHER_H

#include <memory>

#include <GLM/glm.hpp>

#include "../AbstractTeamMember.h"


namespace prop3
{
    class PROP3D_EXPORT AbstractChoreographer : public AbstractTeamMember
    {
    protected:
        AbstractChoreographer() {}

    public:
        virtual ~AbstractChoreographer() {}

        virtual void update(double dt) =0;

        virtual void setGravity(const glm::dvec3& unitsPerSecondSquared) =0;

        virtual void manageProp(const std::shared_ptr<Prop>& prop) =0;
        virtual void unmanageProp(const std::shared_ptr<Prop>& prop) =0;
    };
}

#endif // PROPROOM3D_ABSTRACTCHOREOGRAPHER_H
