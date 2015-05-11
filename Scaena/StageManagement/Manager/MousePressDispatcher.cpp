#include "MousePressDispatcher.h"

#include "../../Play/Character.h"


namespace scaena
{
    MousePressDispatcher::MousePressDispatcher(const MouseEvent& event) :
        _event(event),
        _eventConsumed(false)
    {}

    void MousePressDispatcher::visit(Character& character)
    {
        _eventConsumed |= character.mousePressEvent(_event);
    }
}
