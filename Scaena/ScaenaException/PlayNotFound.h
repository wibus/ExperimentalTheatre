#ifndef SCAENA_PLAY_NOT_FOUND_H
#define SCAENA_PLAY_NOT_FOUND_H

#include "ScaenaException.h"


namespace scaena
{
    class SCAENA_EXPORT PlayNotFound : ScaenaException
    {
    public:
        PlayNotFound(const std::string& what)   throw();
        virtual ~PlayNotFound()                 throw();
    };
}

#endif // SCAENA_PLAY_NOT_FOUND_H
