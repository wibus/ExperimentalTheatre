#ifndef PROPROOM2D_LIBPROPROOM2D_GLOBAL_H
#define PROPROOM2D_LIBPROPROOM2D_GLOBAL_H

#include <CellarWorkbench/DataStructure/Vector.h>
#include <CellarWorkbench/DataStructure/Matrix.h>
#include <CellarWorkbench/Geometry/Segment2D.h>

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
    enum class EPropType
    {
        CIRCLE,
        POLYGON,
        TEXT_HUD,
        IMAGE_HUD,
        NB_TYPES
    };

    enum class EBodyType
    {
        GRAPHIC,
        KINEMATIC,
        DYNAMIC,
        NB_TYPES
    };

    enum class  EHorizontalAnchor
    {
        LEFT,
        RIGHT,
        NB_ANCHORS
    };

    enum class  EVerticalAnchor
    {
        BOTTOM,
        TOP,
        NB_ANCHORS
    };


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
