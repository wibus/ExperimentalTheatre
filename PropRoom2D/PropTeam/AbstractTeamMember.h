#ifndef PROPROOM2D_TEAMMEMBER_H
#define PROPROOM2D_TEAMMEMBER_H

#include "libPropRoom2D_global.h"

namespace prop2
{
    class PROP2D_EXPORT AbstractTeamMember
    {
    protected:
        AbstractTeamMember() {}

    public:
        virtual ~AbstractTeamMember() {}

        virtual void setup() =0;
        virtual void reset() =0;
        virtual void updateViewportSize(int, int) {}
    };
}

#endif // PROPROOM2D_TEAMMEMBER_H
