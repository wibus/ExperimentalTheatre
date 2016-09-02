#ifndef PROPROOM2D_BOUND_H
#define PROPROOM2D_BOUND_H

#include "PropRoom2D/libPropRoom2D_global.h"


namespace prop2
{
    class PROP2D_EXPORT Bound
    {
    public:
        Bound(int id, int e);
        ~Bound();

        int id;
        int e;

    private:
    };



    // IMPLEMENTATION //
}

#endif // PROPROOM2D_BOUND_H
