#ifndef SCAENA_SYNCHRONOUSMOUSE_H
#define SCAENA_SYNCHRONOUSMOUSE_H

#include <CellarWorkbench/DataStructure/Vector.h>

#include "../../libScaena_global.h"
#include "EventTypes.h"


namespace scaena
{
    class SCAENA_EXPORT SynchronousMouse
    {
    public:
        SynchronousMouse();

        void setPosition(const cellar::Vec2i& position);
        void setButtonIsPressed(EMouseButton button, bool isPressed);

        int x() const;
        int y() const;
        cellar::Vec2i position() const;
        int xDisplacement() const;
        int yDisplacement() const;
        cellar::Vec2i displacement() const;
        bool buttonIsPressed(EMouseButton button) const;

    private:
        cellar::Vec2i _position;
        cellar::Vec2i _displacement;
        bool _buttonPressed[(int)EMouseButton::NB_BUTTONS];
    };



    // IMPLEMENTATION //
    inline void SynchronousMouse::setButtonIsPressed(EMouseButton button, bool isPressed)
        {_buttonPressed[(int)button] = isPressed;}
    inline int SynchronousMouse::x() const {return _position.x();}
    inline int SynchronousMouse::y() const {return _position.y();}
    inline cellar::Vec2i SynchronousMouse::position() const {return _position;}
    inline int SynchronousMouse::xDisplacement() const {return _displacement.x();}
    inline int SynchronousMouse::yDisplacement() const {return _displacement.y();}
    inline cellar::Vec2i SynchronousMouse::displacement() const {return _displacement;}
    inline bool SynchronousMouse::buttonIsPressed(EMouseButton button) const {return _buttonPressed[(int)button];}
}

#endif // SCAENA_SYNCHRONOUSMOUSE_H
