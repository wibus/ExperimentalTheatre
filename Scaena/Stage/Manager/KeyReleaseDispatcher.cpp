#include "KeyReleaseDispatcher.h"
#include "Character/AbstractCharacter.h"


namespace scaena
{
    KeyReleaseDispatcher::KeyReleaseDispatcher(const KeyboardEvent& event) :
        _event(event)
    {}

    void KeyReleaseDispatcher::visit(AbstractCharacter& character)
    {
        character.keyReleaseEvent(_event);
    }
}
