#include "KeyboardEvent.h"

#include <cctype>


namespace scaena
{
    KeyboardEvent::KeyboardEvent(char ascii, Keyboard::NonAscii nonAscii) :
        _ascii(toupper(ascii)),
        _nonAscii(nonAscii)
    {}
}
