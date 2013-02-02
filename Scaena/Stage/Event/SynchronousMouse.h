#ifndef SCAENA_SYNCHRONOUSMOUSE_H
#define SCAENA_SYNCHRONOUSMOUSE_H

#include <DataStructure/Vector2D.h>

#include "libScaena_global.h"
#include "EventTypes.h"


namespace scaena
{
    class SCAENA_EXPORT SynchronousMouse
    {
    public:
        SynchronousMouse();

        void setPosition(const cellar::Vector2D<int> position);
        void setButtonIsPressed(Mouse::Button button, bool isPressed);

        int x() const;
        int y() const;
        cellar::Vector2D<int> position() const;
        int xDisplacement() const;
        int yDisplacement() const;
        cellar::Vector2D<int> displacement() const;
        bool buttonIsPressed(Mouse::Button button) const;

    private:
        cellar::Vector2D<int> _position;
        cellar::Vector2D<int> _displacement;
        bool _buttonPressed[Mouse::NB_BUTTONS];
    };



    // IMPLEMENTATION //
    inline void SynchronousMouse::setButtonIsPressed(Mouse::Button button, bool isPressed)
        {_buttonPressed[button] = isPressed;}
    inline int SynchronousMouse::x() const {return _position.x();}
    inline int SynchronousMouse::y() const {return _position.y();}
    inline cellar::Vector2D<int> SynchronousMouse::position() const {return _position;}
    inline int SynchronousMouse::xDisplacement() const {return _displacement.x();}
    inline int SynchronousMouse::yDisplacement() const {return _displacement.y();}
    inline cellar::Vector2D<int> SynchronousMouse::displacement() const {return _displacement;}
    inline bool SynchronousMouse::buttonIsPressed(Mouse::Button button) const {return _buttonPressed[button];}
}

#endif // SCAENA_SYNCHRONOUSMOUSE_H
