#include "MouseMoveDispatcher.h"

#include "../../Play/Character.h"


namespace scaena
{
    MouseMoveDispatcher::MouseMoveDispatcher(const MouseEvent& event) :
        _event(event),
        _eventConsumed(false)
    {}

    void MouseMoveDispatcher::visit(Character& character)
    {
        _eventConsumed |= character.mouseMoveEvent(_event);
    }
}
