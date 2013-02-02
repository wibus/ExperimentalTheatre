#ifndef SCAENA_KEYBOARD_EVENT_H
#define SCAENA_KEYBOARD_EVENT_H

#include "libScaena_global.h"
#include "EventTypes.h"

namespace scaena
{
    class SCAENA_EXPORT KeyboardEvent
    {
    public:
        KeyboardEvent(char ascii, Keyboard::NonAscii nonAscii);

        char getAscii()     const;
        Keyboard::NonAscii getNonAscii()  const;


    private :
        char _ascii;
        Keyboard::NonAscii _nonAscii;
    };



    // IMPLEMENTATION //
    inline char KeyboardEvent::getAscii() const
        {return _ascii;}

    inline Keyboard::NonAscii KeyboardEvent::getNonAscii() const
        {return _nonAscii;}
}

#endif // SCAENA_KEYBOARD_EVENT_H
