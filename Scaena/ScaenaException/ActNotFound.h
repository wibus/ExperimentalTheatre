#ifndef SCAENA_ACT_NOT_FOUND_H
#define SCAENA_ACT_NOT_FOUND_H

#include "ScaenaException.h"


namespace scaena
{
    class SCAENA_EXPORT ActNotFound : ScaenaException
    {
    public :
        ActNotFound(const std::string& what)    throw();
        virtual ~ActNotFound()                  throw();
    };
}

#endif // SCAENA_ACT_NOT_FOUND_H
