#include "MouseEvent.h"


namespace scaena
{
    MouseEvent::MouseEvent(Mouse::Button button,
                           const cellar::Vector2D<int>& pos,
                           const cellar::Vector2D<int>& globalPos) :
        _button(button),
        _position(pos),
        _globalPosition(globalPos)
    {}
}
