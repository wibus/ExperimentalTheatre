#ifndef SCAENA_SYNCHRONOUSMOUSE_H
#define SCAENA_SYNCHRONOUSMOUSE_H

#include <GLM/glm.hpp>

#include "../../libScaena_global.h"
#include "EventTypes.h"


namespace scaena
{
    class SCAENA_EXPORT SynchronousMouse
    {
    public:
        SynchronousMouse();

        void setPosition(const glm::ivec2& position);
        void setButtonIsPressed(EMouseButton button, bool isPressed);

        int x() const;
        int y() const;
        glm::ivec2 position() const;
        int xDisplacement() const;
        int yDisplacement() const;
        glm::ivec2 displacement() const;
        bool buttonIsPressed(EMouseButton button) const;

    private:
        glm::ivec2 _position;
        glm::ivec2 _displacement;
        bool _buttonPressed[(int)EMouseButton::NB_BUTTONS];
    };



    // IMPLEMENTATION //
    inline void SynchronousMouse::setButtonIsPressed(EMouseButton button, bool isPressed)
        {_buttonPressed[(int)button] = isPressed;}
    inline int SynchronousMouse::x() const {return _position.x;}
    inline int SynchronousMouse::y() const {return _position.y;}
    inline glm::ivec2 SynchronousMouse::position() const {return _position;}
    inline int SynchronousMouse::xDisplacement() const {return _displacement.x;}
    inline int SynchronousMouse::yDisplacement() const {return _displacement.y;}
    inline glm::ivec2 SynchronousMouse::displacement() const {return _displacement;}
    inline bool SynchronousMouse::buttonIsPressed(EMouseButton button) const {return _buttonPressed[(int)button];}
}

#endif // SCAENA_SYNCHRONOUSMOUSE_H
