#include "BeginStepCaller.h"

#include "../../Play/Character.h"


namespace scaena
{
    BeginStepCaller::BeginStepCaller(const StageTime& time) :
        _time(time)
    {}

    void BeginStepCaller::visit(Character& character)
    {
        if(character.updates())
            character.beginStep(_time);
    }
}
