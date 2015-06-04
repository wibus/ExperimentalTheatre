#include "SynchronousMouse.h"

#include <cstring>


namespace scaena
{
    SynchronousMouse::SynchronousMouse() :
        _degreeDelta(0),
        _position(0, 0),
        _displacement(0, 0)
    {
        memset(_buttonPressed, false, (int)EMouseButton::NB_BUTTONS * sizeof(bool));
    }

    void SynchronousMouse::setDegreeDelta(int degreeDelta)
    {
        _degreeDelta = degreeDelta;
    }

    void SynchronousMouse::setButtonIsPressed(EMouseButton button, bool isPressed)
    {
        _buttonPressed[(int)button] = isPressed;
    }

    void SynchronousMouse::setPosition(const glm::ivec2& position)
    {
        _displacement = position - _position;
        _position = position;
    }
}
