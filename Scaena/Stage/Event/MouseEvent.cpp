#include "MouseEvent.h"


namespace scaena
{
    MouseEvent::MouseEvent(Mouse::Button button,
                           const cellar::Vec2i& pos,
                           const cellar::Vec2i& globalPos) :
        _button(button),
        _position(pos),
        _globalPosition(globalPos)
    {}
}
