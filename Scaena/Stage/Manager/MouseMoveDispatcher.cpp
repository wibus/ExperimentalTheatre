#include "MouseMoveDispatcher.h"

#include "../../Character/AbstractCharacter.h"


namespace scaena
{
    MouseMoveDispatcher::MouseMoveDispatcher(const MouseEvent& event) :
        _event(event)
    {}

    void MouseMoveDispatcher::visit(AbstractCharacter& character)
    {
        character.mouseMoveEvent(_event);
    }
}
