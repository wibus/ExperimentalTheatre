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
        bool isNonAsciiPressed(ENonAscii c) const;

        void setIsAsciiPressed(char c, bool isPressed);
        void setIsNonAsciiPressed(ENonAscii c, bool isPressed);

    private:
        bool _isAsciiPressed[256];
        bool _isNonAsciiPressed[(int)ENonAscii::NB_NONASCIIS];
    };



    // IMPLEMENTATION //
    inline bool SynchronousKeyboard::isAsciiPressed(char c) const
        {return _isAsciiPressed[toupper(c)];}
    inline bool SynchronousKeyboard::isNonAsciiPressed(ENonAscii c) const
        {return _isNonAsciiPressed[(int)c];}
    inline void SynchronousKeyboard::setIsAsciiPressed(char c, bool isPressed)
        {_isAsciiPressed[toupper(c)] = isPressed;}
    inline void SynchronousKeyboard::setIsNonAsciiPressed(ENonAscii c, bool isPressed)
        {_isNonAsciiPressed[(int)c] = isPressed;}
}

#endif // SCAENA_SYNCHRONOUSKEYBOARD_H
