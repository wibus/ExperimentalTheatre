#include "KeyboardEvent.h"

#include <cctype>

#include <QKeyEvent>


namespace scaena
{
    KeyboardEvent::KeyboardEvent(char ascii, ENonAscii nonAscii) :
        _ascii(toupper(ascii)),
        _nonAscii(nonAscii)
    {}

    KeyboardEvent KeyboardEvent::convertQKeyEvent(QKeyEvent * event)
    {
        char ascii = '\0';
        ENonAscii nonAscii = ENonAscii::NO_NONASCII;

        if(event->text().toStdString().size() != 0)
            ascii = event->text().toStdString().at(0);

        else
            switch(event->key())
            {
            case Qt::Key_Shift :    nonAscii = ENonAscii::SHIFT;   break;
            case Qt::Key_Control :  nonAscii = ENonAscii::CTRL;    break;
            case Qt::Key_Tab :      nonAscii = ENonAscii::TAB;     break;

            case Qt::Key_Alt :      nonAscii = ENonAscii::ALT;     break;
            case Qt::Key_AltGr :    nonAscii = ENonAscii::ALT_CAR; break;
            case Qt::Key_Super_L :  nonAscii = ENonAscii::SUPER;   break;
            case Qt::Key_Super_R :  nonAscii = ENonAscii::SUPER;   break;
            case Qt::Key_Escape :   nonAscii = ENonAscii::ESCAPE;  break;

            case Qt::Key_Up :       nonAscii = ENonAscii::UP;      break;
            case Qt::Key_Down :     nonAscii = ENonAscii::DOWN;    break;
            case Qt::Key_Left :     nonAscii = ENonAscii::LEFT;    break;
            case Qt::Key_Right :    nonAscii = ENonAscii::RIGHT;   break;

            case Qt::Key_Space :    nonAscii = ENonAscii::SPACE;   break;

            default :               nonAscii = ENonAscii::NO_NONASCII;
            }

        return KeyboardEvent(ascii, nonAscii);
    }
}
