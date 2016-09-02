#include "Edge.h"


namespace prop2
{
    Edge::Edge(int v0, int v1, int b) :
        v{v0, v1},
        bound(b)
    {

    }

    Edge::~Edge()
    {

    }
}
