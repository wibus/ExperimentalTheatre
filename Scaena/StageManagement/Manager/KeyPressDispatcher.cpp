#include "KeyPressDispatcher.h"

#include "../../Play/Character.h"


namespace scaena
{
    KeyPressDispatcher::KeyPressDispatcher(const KeyboardEvent& event) :
        _event(event),
        _eventConsumed(false)
    {}

    void KeyPressDispatcher::visit(Character& character)
    {
        _eventConsumed |= character.keyPressEvent(_event);
    }
}
