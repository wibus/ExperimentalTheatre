#ifndef SCAENA_STAGE_NOT_FOUND_H
#define SCAENA_STAGE_NOT_FOUND_H

#include "ScaenaException.h"


namespace scaena
{
    class SCAENA_EXPORT StageNotFound : public ScaenaException
    {
    public:
        StageNotFound(const std::string& what)  throw();
        virtual ~StageNotFound()                throw();
    };
}

#endif // SCAENA_STAGE_NOT_FOUND_H
