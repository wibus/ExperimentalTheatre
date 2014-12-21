#include "MousePressDispatcher.h"

#include "../../Character/AbstractCharacter.h"


namespace scaena
{
    MousePressDispatcher::MousePressDispatcher(const MouseEvent& event) :
        _event(event)
    {}

    void MousePressDispatcher::visit(AbstractCharacter& character)
    {
        character.mousePressEvent(_event);
    }
}
