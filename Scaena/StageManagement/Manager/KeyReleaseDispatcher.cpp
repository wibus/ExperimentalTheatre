#include "KeyReleaseDispatcher.h"

#include "../../Play/Character.h"


namespace scaena
{
    KeyReleaseDispatcher::KeyReleaseDispatcher(const KeyboardEvent& event) :
        _event(event),
        _eventConsumed(false)
    {}

    void KeyReleaseDispatcher::visit(Character& character)
    {
        _eventConsumed |= character.keyReleaseEvent(_event);
    }
}
