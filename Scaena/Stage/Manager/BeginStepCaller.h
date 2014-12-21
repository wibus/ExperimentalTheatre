#ifndef SCAENA_BEGIN_STEP_CALLER_H
#define SCAENA_BEGIN_STEP_CALLER_H

#include "StageManager.h"
#include "../Event/StageTime.h"


namespace scaena
{
    class SCAENA_EXPORT BeginStepCaller : public StageManager
    {
    public:
        BeginStepCaller(const StageTime& time);

        virtual void visit(AbstractCharacter& character);


    private :
        StageTime _time;
    };
}

#endif // SCAENA_BEGIN_STEP_CALLER_H
