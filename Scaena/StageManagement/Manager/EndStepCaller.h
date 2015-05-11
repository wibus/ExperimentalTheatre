#ifndef SCAENA_END_STEP_CALLER_H
#define SCAENA_END_STEP_CALLER_H

#include "StageManager.h"
#include "../Event/StageTime.h"


namespace scaena
{
    class SCAENA_EXPORT EndStepCaller : public StageManager
    {
    public:
        EndStepCaller(const StageTime& time);

        virtual void visit(Character& character);


    private :
        StageTime _time;
    };
}

#endif // SCAENA_END_STEP_CALLER_H
