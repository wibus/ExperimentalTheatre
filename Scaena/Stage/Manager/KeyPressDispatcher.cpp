#include "KeyPressDispatcher.h"
#include "Character/AbstractCharacter.h"


namespace scaena
{
    KeyPressDispatcher::KeyPressDispatcher(const KeyboardEvent& event) :
        _event(event)
    {}

    void KeyPressDispatcher::visit(AbstractCharacter& character)
    {
        character.keyPressEvent(_event);
    }
}
