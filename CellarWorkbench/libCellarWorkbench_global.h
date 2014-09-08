#ifndef CELLARWORKBENCH_LIBCELLARWORKBENCH_GLOBAL_H
#define CELLARWORKBENCH_LIBCELLARWORKBENCH_GLOBAL_H

#ifdef _WINDOWS
#   if defined(CELLARWORKBENCH_LIBRARY)
#       define CELLAR_EXPORT __declspec(dllexport)
#   else
#       define CELLAR_EXPORT __declspec(dllimport)
#   endif
#else
#   define CELLAR_EXPORT
#endif

/// \namespace cellar
/// \brief All the classes and functions of the Cellar Workbench module
namespace cellar {}

#endif // CELLARWORKBENCH_LIBCELLARWORKBENCH_GLOBAL_H
