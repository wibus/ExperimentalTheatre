#ifndef PROPROOM3D_TEAMMEMBER_H
#define PROPROOM3D_TEAMMEMBER_H

#include "libPropRoom3D_global.h"

namespace prop3
{
class Brep;
class Mesh;

    class PROP3D_EXPORT AbstractTeamMember
    {
    protected:
        AbstractTeamMember() {}

    public:
        virtual ~AbstractTeamMember() {}

        virtual void setup() =0;
        virtual void reset() =0;
    };
}

#endif // PROPROOM3D_TEAMMEMBER_H
