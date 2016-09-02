#ifndef PROPROOM2D_VERT_H
#define PROPROOM2D_VERT_H

#include <GLM/glm.hpp>

#include "PropRoom2D/libPropRoom2D_global.h"


namespace prop2
{
    // Vert::n imposes the normal at this vertex
    // if Vert::n == null vector, there's a discontinuity in the silouet normals

    class PROP2D_EXPORT Vert
    {
    public:
        Vert(const glm::dvec2& p, const glm::dvec2& n = glm::dvec2());
        ~Vert();

        glm::dvec2 p;
        glm::dvec2 n;

    private:
    };



    // IMPLEMENTATION //
}

#endif // PROPROOM2D_VERT_H
