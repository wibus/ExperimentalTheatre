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

namespace prop3
{

}

#endif // PROPROOM3D_LIBPROPROOM3D_GLOBAL_H
