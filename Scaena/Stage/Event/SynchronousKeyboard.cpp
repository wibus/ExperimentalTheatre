#include "SynchronousKeyboard.h"

#include <cstring>

namespace scaena
{
    SynchronousKeyboard::SynchronousKeyboard()
    {
        memset(_isAsciiPressed, false, 256 * sizeof(bool));
        memset(_isNonAsciiPressed, false, Keyboard::NB_NONASCIIS * sizeof(bool));
    }
}
