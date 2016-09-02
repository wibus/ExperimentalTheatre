#ifndef PROPROOM2D_MESH_H
#define PROPROOM2D_MESH_H

#include <vector>

#include "Vert.h"
#include "Edge.h"
#include "Tri.h"

#include "Bound.h"
#include "Interface.h"

#include "PropRoom2D/libPropRoom2D_global.h"


namespace prop2
{
    class PROP2D_EXPORT Mesh
    {
    public:
        Mesh();
        ~Mesh();

        std::vector<Vert> verts;
        std::vector<Edge> edges;
        std::vector<Tri> tris;

        std::vector<Bound> bounds;
        std::vector<Interface> interfaces;

    private:
    };



    // IMPLEMENTATION //
}

#endif // PROPROOM2D_MESH_H
