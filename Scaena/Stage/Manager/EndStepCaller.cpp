#include "EndStepCaller.h"
#include "Character/AbstractCharacter.h"


namespace scaena
{
    EndStepCaller::EndStepCaller(const StageTime& time) :
        _time(time)
    {}

    void EndStepCaller::visit(AbstractCharacter& character)
    {
        if(character.updates()) character.endStep(_time);
    }
}
