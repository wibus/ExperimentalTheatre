#ifndef LIBPROPROOM3D_GLOBAL_H
#define LIBPROPROOM3D_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBPROPROOM3D_LIBRARY)
#  define PROP3D_EXPORT Q_DECL_EXPORT
#else
#  define PROP3D_EXPORT Q_DECL_IMPORT
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
