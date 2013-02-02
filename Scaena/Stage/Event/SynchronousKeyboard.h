#ifndef SCAENA_SYNCHRONOUSKEYBOARD_H
#define SCAENA_SYNCHRONOUSKEYBOARD_H

#include <ctype.h>

#include "libScaena_global.h"
#include "EventTypes.h"


namespace scaena
{
    class SynchronousKeyboard
    {
    public:
        SynchronousKeyboard();

        bool isAsciiPressed(char c) const;
        bool isNonAsciiPressed(Keyboard::NonAscii c) const;

        void setIsAsciiPressed(char c, bool isPressed);
        void setIsNonAsciiPressed(Keyboard::NonAscii c, bool isPressed);

    private:
        bool _isAsciiPressed[256];
        bool _isNonAsciiPressed[Keyboard::NB_NONASCIIS];
    };



    // IMPLEMENTATION //
    inline bool SynchronousKeyboard::isAsciiPressed(char c) const
        {return _isAsciiPressed[toupper(c)];}
    inline bool SynchronousKeyboard::isNonAsciiPressed(Keyboard::NonAscii c) const
        {return _isNonAsciiPressed[toupper(c)];}
    inline void SynchronousKeyboard::setIsAsciiPressed(char c, bool isPressed)
        {_isAsciiPressed[(int)c] = isPressed;}
    inline void SynchronousKeyboard::setIsNonAsciiPressed(Keyboard::NonAscii c, bool isPressed)
        {_isNonAsciiPressed[(int)c] = isPressed;}
}

#endif // SCAENA_SYNCHRONOUSKEYBOARD_H
