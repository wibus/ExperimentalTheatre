#ifndef SCAENA_DRAW_CALLER_H
#define SCAENA_DRAW_CALLER_H

#include "StageManager.h"
#include "../Event/StageTime.h"


namespace scaena
{
    class SCAENA_EXPORT DrawCaller : public StageManager
    {
    public:
        DrawCaller(const StageTime& time);

        virtual void visit(AbstractCharacter& character);


    private :
        StageTime _time;
    };
}

#endif // SCAENA_DRAW_CALLER_H
