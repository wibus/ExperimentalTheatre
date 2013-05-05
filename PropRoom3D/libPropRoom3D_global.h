#ifndef LIBPROPROOM3D_GLOBAL_H
#define LIBPROPROOM3D_GLOBAL_H

#ifdef _WINDOWS
#   if defined(PROPROOM3D_LIBRARY)
#       define PROP3D_EXPORT __declspec(dllexport)
#   else
#       define PROP3D_EXPORT __declspec(dllimport)
#   endif
#else
#   define PROP3D_EXPORT
#endif

namespace prop3
{
    // Shapes attributes
    typedef unsigned int PropId;
    namespace PropType
    {
    enum Enum {NB_TYPES};
    }
}

#endif // LIBPROPROOM3D_GLOBAL_H
