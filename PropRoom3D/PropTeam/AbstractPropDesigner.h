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

        virtual std::shared_ptr<Prop> createProp() = 0;
    };
}

#endif // PROPROOM3D_ABSTRACTPROPDESIGNER_H
