#ifndef PROPROOM2D_EDGE_H
#define PROPROOM2D_EDGE_H

#include "PropRoom2D/libPropRoom2D_global.h"


namespace prop2
{
    class PROP2D_EXPORT Edge
    {
    public:
        Edge(int v0, int v1, int b);
        ~Edge();

        int v[2];
        int bound;

    private:
    };



    // IMPLEMENTATION //
}

#endif // PROPROOM2D_EDGE_H
