#ifndef SCAENA_STAGE_MANAGER_H
#define SCAENA_STAGE_MANAGER_H

#include "libScaena_global.h"


namespace scaena
{
    class AbstractCharacter;


    class SCAENA_EXPORT StageManager
    {
    public:
        StageManager() {}
        virtual ~StageManager() {}

        virtual void visit(AbstractCharacter& character) =0;
    };
}

#endif // SCAENA_STAGE_MANAGER_H
