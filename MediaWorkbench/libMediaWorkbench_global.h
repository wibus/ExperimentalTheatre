#ifndef MEDIAWORKBENCH_LIBMEDIAWORKBENCH_GLOBAL_H
#define MEDIAWORKBENCH_LIBMEDIAWORKBENCH_GLOBAL_H

#ifdef _WINDOWS
#   if defined(MEDIAWORKBENCH_LIBRARY)
#       define MEDIA_EXPORT __declspec(dllexport)
#   else
#       define MEDIA_EXPORT __declspec(dllimport)
#   endif
#else
#   define MEDIA_EXPORT
#endif

#endif // MEDIAWORKBENCH_LIBMEDIAWORKBENCH_GLOBAL_H
