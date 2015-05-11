#include "MouseReleaseDispatcher.h"

#include "../../Play/Character.h"


namespace scaena
{
    MouseReleaseDispatcher::MouseReleaseDispatcher(const MouseEvent& event) :
        _event(event),
        _eventConsumed(false)
    {}

    void MouseReleaseDispatcher::visit(Character& character)
    {
        _eventConsumed |= character.mouseReleaseEvent(_event);
    }
}
