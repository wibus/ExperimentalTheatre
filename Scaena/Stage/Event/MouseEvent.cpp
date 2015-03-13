#include "MouseEvent.h"


namespace scaena
{
    MouseEvent::MouseEvent(EMouseButton button,
                           const glm::ivec2& pos,
                           const glm::ivec2& globalPos) :
        _button(button),
        _position(pos),
        _globalPosition(globalPos)
    {}
}
