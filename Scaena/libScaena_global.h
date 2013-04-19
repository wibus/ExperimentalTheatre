#ifndef SCAENA_LIBSCAENA_GLOBAL_H
#define SCAENA_LIBSCAENA_GLOBAL_H

#ifdef _WINDOWS
#   if defined(SCAENA_LIBRARY)
#       define SCAENA_EXPORT __declspec(dllexport)
#   else
#       define SCAENA_EXPORT __declspec(dllimport)
#   endif
#endif

#endif // SCAENA_LIBSCAENA_GLOBAL_H
