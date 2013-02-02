#ifndef SCAENA_EVENTTYPES_H
#define SCAENA_EVENTTYPES_H


namespace scaena
{
    namespace Mouse
    {
        enum Button {NO_BUTTON,
                     LEFT,
                     MIDDLE,
                     RIGHT,
                     NB_BUTTONS};
    }

    namespace Keyboard
    {
        enum NonAscii {NO_NONASCII, SHIFT,          CTRL,       TAB,
                       ALT,         ALT_CAR,        SUPER,      ESCAPE,
                       UP,          DOWN,           LEFT,       RIGHT,
                       SPACE,       NB_NONASCIIS};
    }
}

#endif // SCAENA_EVENTTYPES_H
