#ifndef PROPROOM2D_LIBPROPROOM2D_GLOBAL_H
#define PROPROOM2D_LIBPROPROOM2D_GLOBAL_H

#include <DataStructure/Vector.h>
#include <DataStructure/Matrix.h>
#include <Geometry/Segment2D.h>

#ifdef _WINDOWS
#   if defined(PROPROOM2D_LIBRARY)
#       define PROP2D_EXPORT __declspec(dllexport)
#   else
#       define PROP2D_EXPORT __declspec(dllimport)
#   endif
#else
#   define PROP2D_EXPORT
#endif

namespace prop2
{
    // Units precision
    typedef float real;


    // Shapes attributes
    typedef unsigned int PropId;
    namespace PropType
    {
    enum Enum {CIRCLE,
               POLYGON,
               TEXT_HUD,
               IMAGE_HUD,
               NB_TYPES};
    }

    namespace BodyType
    {
    enum Enum {STATIC,
               CINEMATIC,
               DYNAMIC,
               NB_TYPES};
    }

    namespace HorizontalAnchor
    {
    enum Enum {LEFT,
               RIGHT,
               NB_ANCHORS};
    }

    namespace VerticalAnchor
    {
    enum Enum {BOTTOM,
               TOP,
               NB_ANCHORS};
    }


    // Vectors and matrices
    typedef cellar::Vector<2, real> Vec2r;
    typedef cellar::Vector<3, real> Vec3r;
    typedef cellar::Vector<4, real> Vec4r;
    typedef cellar::Matrix<3, real> Mat3r;
    typedef cellar::Matrix<4, real> Mat4r;

    // Line
    typedef cellar::Segment2D<real> Segment2Dr;
}

#endif // PROPROOM2D_LIBPROPROOM2D_GLOBAL_H
