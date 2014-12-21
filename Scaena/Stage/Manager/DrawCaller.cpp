#include "DrawCaller.h"

#include "../../Character/AbstractCharacter.h"


namespace scaena
{
    DrawCaller::DrawCaller(const StageTime& time) :
        _time(time)
    {}

    void DrawCaller::visit(AbstractCharacter& character)
    {
        if(character.visible()) character.draw(_time);
    }
}
