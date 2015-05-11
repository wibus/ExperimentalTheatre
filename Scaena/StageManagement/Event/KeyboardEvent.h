#ifndef SCAENA_KEYBOARD_EVENT_H
#define SCAENA_KEYBOARD_EVENT_H

#include "../../libScaena_global.h"
#include "EventTypes.h"

class QKeyEvent;


namespace scaena
{
    class SCAENA_EXPORT KeyboardEvent
    {
    public:
        KeyboardEvent(char ascii, ENonAscii nonAscii);

        char getAscii()     const;
        ENonAscii getNonAscii()  const;

        static KeyboardEvent convertQKeyEvent(QKeyEvent * event);

    private :
        char _ascii;
        ENonAscii _nonAscii;
    };



    // IMPLEMENTATION //
    inline char KeyboardEvent::getAscii() const
        {return _ascii;}

    inline ENonAscii KeyboardEvent::getNonAscii() const
        {return _nonAscii;}
}

#endif // SCAENA_KEYBOARD_EVENT_H
