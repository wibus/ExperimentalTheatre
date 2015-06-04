#include "MouseWheelDispatcher.h"

#include "../../Play/Character.h"


namespace scaena
{
    MouseWheelDispatcher::MouseWheelDispatcher(const MouseEvent& event) :
        _event(event),
        _eventConsumed(false)
    {}

    void MouseWheelDispatcher::visit(Character& character)
    {
        _eventConsumed |= character.mouseWheelEvent(_event);
    }
}
