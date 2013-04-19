#ifndef CELLARWORKBENCH_LIBCELLARWORKBENCH_GLOBAL_H
#define CELLARWORKBENCH_LIBCELLARWORKBENCH_GLOBAL_H

#ifdef _WINDOWS
#   if defined(CELLARWORKBENCH_LIBRARY)
#       define CELLAR_EXPORT __declspec(dllexport)
#   else
#       define CELLAR_EXPORT __declspec(dllimport)
#   endif
#endif

#endif // CELLARWORKBENCH_LIBCELLARWORKBENCH_GLOBAL_H
