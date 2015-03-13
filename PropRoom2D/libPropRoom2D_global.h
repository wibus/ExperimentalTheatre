#ifndef PROPROOM2D_LIBPROPROOM2D_GLOBAL_H
#define PROPROOM2D_LIBPROPROOM2D_GLOBAL_H

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
}

#endif // PROPROOM2D_LIBPROPROOM2D_GLOBAL_H
