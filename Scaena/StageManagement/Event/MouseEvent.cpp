#include "MouseEvent.h"

#include <QMouseEvent>


namespace scaena
{
    MouseEvent::MouseEvent(EMouseButton button,
                           const glm::ivec2& pos,
                           const glm::ivec2& globalPos) :
        _button(button),
        _position(pos),
        _globalPosition(globalPos)
    {}

    MouseEvent MouseEvent::convertQMouseEvent(QMouseEvent* event)
    {
        glm::ivec2 pos(event->x(), event->y());
        glm::ivec2 globalPos(event->globalX(), event->globalY());

        EMouseButton button = EMouseButton::NO_BUTTON;

        switch(event->button())
        {
        case Qt::LeftButton :   button = EMouseButton::LEFT;      break;
        case Qt::MiddleButton : button = EMouseButton::MIDDLE;    break;
        case Qt::RightButton :  button = EMouseButton::RIGHT;     break;
        default :               button = EMouseButton::NO_BUTTON;
        }

        return MouseEvent(button, pos, globalPos);
    }
}
