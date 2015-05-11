#include "EndStepCaller.h"

#include "../../Play/Character.h"


namespace scaena
{
    EndStepCaller::EndStepCaller(const StageTime& time) :
        _time(time)
    {}

    void EndStepCaller::visit(Character& character)
    {
        if(character.updates())
            character.endStep(_time);
    }
}
