#ifndef PROPROOM3D_LIBPROPROOM3D_GLOBAL_H
#define PROPROOM3D_LIBPROPROOM3D_GLOBAL_H

#ifdef _WINDOWS
#   if defined(PROPROOM3D_LIBRARY)
#       define PROP3D_EXPORT __declspec(dllexport)
#   else
#       define PROP3D_EXPORT __declspec(dllimport)
#   endif
#else
#   define PROP3D_EXPORT
#endif

#define GLM_FORCE_RADIANS

namespace prop3
{
    // Shapes attributes
    typedef unsigned int PropId;
    
    // Prop types
    enum class EPropType
    {
        MESH,
        BREP,
        NB_TYPES
    };

    // Body types
    enum class EBodyType
    {
        GRAPHIC,
        KINEMATIC,
        DYNAMIC,
        NB_TYPES
    };
}

#endif // PROPROOM3D_LIBPROPROOM3D_GLOBAL_H
