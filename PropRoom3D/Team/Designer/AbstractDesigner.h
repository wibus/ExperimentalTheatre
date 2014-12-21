#ifndef PROPROOM3D_ABSTRACTDESIGNER_H
#define PROPROOM3D_ABSTRACTDESIGNER_H

#include <memory>

#include "../AbstractTeamMember.h"


namespace prop3
{
    class PROP3D_EXPORT AbstractDesigner : public AbstractTeamMember
    {
    protected:
        AbstractDesigner() {}

    public:
        virtual ~AbstractDesigner() {}

        virtual std::shared_ptr<Prop> createProp() = 0;
    };
}

#endif // PROPROOM3D_ABSTRACTDESIGNER_H
