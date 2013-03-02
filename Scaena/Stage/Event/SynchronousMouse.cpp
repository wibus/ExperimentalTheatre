#include "SynchronousMouse.h"

#include <cstring>


namespace scaena
{
    SynchronousMouse::SynchronousMouse() :
        _position(0, 0),
        _displacement(0, 0)
    {
        memset(_buttonPressed, false, Mouse::NB_BUTTONS * sizeof(bool));
    }

    void SynchronousMouse::setPosition(const cellar::Vec2i& position)
    {
        _displacement = position - _position;
        _position = position;
    }
}
