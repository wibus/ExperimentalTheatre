#include "BeginStepCaller.h"

#include "../../Character/AbstractCharacter.h"


namespace scaena
{
    BeginStepCaller::BeginStepCaller(const StageTime& time) :
        _time(time)
    {}

    void BeginStepCaller::visit(AbstractCharacter& character)
    {
        if(character.updates()) character.beginStep(_time);
    }
}
