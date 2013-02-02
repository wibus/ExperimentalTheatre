#include "MouseReleaseDispatcher.h"
#include "Character/AbstractCharacter.h"


namespace scaena
{
    MouseReleaseDispatcher::MouseReleaseDispatcher(const MouseEvent& event) :
        _event(event)
    {}

    void MouseReleaseDispatcher::visit(AbstractCharacter& character)
    {
        character.mouseReleaseEvent(_event);
    }
}
