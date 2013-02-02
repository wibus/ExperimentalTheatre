#ifndef SCAENA_CHARACTER_NOT_FOUND_H
#define SCAENA_CHARACTER_NOT_FOUND_H

#include "ScaenaException.h"


namespace scaena
{
    class SCAENA_EXPORT CharacterNotFound : ScaenaException
    {
    public :
        CharacterNotFound(const std::string& what)  throw();
        virtual ~CharacterNotFound()                throw();
    };
}

#endif // SCAENA_CHARACTER_NOT_FOUND_H
