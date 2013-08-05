#include "KeyboardEvent.h"

#include <cctype>


namespace scaena
{
    KeyboardEvent::KeyboardEvent(char ascii, ENonAscii nonAscii) :
        _ascii(toupper(ascii)),
        _nonAscii(nonAscii)
    {}
}
