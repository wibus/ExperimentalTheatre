#ifndef PROPROOM3D_STDTEAM_H
#define PROPROOM3D_STDTEAM_H

#include "AbstractTeam.h"


namespace prop3
{
    class PROP3D_EXPORT DummyTeam : public AbstractTeam
    {
    public:
        DummyTeam();
        virtual ~DummyTeam();
    };
}

#endif // PROPROOM3D_STDTEAM_H
