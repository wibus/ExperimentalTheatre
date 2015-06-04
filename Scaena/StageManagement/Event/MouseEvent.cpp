#include "MouseEvent.h"

#include <QMouseEvent>
#include <QWheelEvent>


namespace scaena
{
    MouseEvent::MouseEvent(EMouseButton button,
                           const glm::ivec2& pos,
                           const glm::ivec2& globalPos,
                           int degreeDelta) :
        _button(button),
        _position(pos),
        _globalPosition(globalPos),
        _degreeDelta(degreeDelta)
    {}

    MouseEvent MouseEvent::convertQMouseEvent(QMouseEvent* event)
    {
        glm::ivec2 pos(event->x(), event->y());
        glm::ivec2 globalPos(event->globalX(), event->globalY());
        int degreeDelta = 0;

        EMouseButton button = EMouseButton::NO_BUTTON;

        switch(event->button())
        {
        case Qt::LeftButton :   button = EMouseButton::LEFT;      break;
        case Qt::MiddleButton : button = EMouseButton::MIDDLE;    break;
        case Qt::RightButton :  button = EMouseButton::RIGHT;     break;
        default :               button = EMouseButton::NO_BUTTON;
        }

        return MouseEvent(button, pos, globalPos, degreeDelta);
    }

    MouseEvent MouseEvent::convertQWheelEvent(QWheelEvent* event)
    {
        glm::ivec2 pos(event->x(), event->y());
        glm::ivec2 globalPos(event->globalX(), event->globalY());
        int degreeDelta = event->angleDelta().y() / 8;
        EMouseButton button = EMouseButton::NO_BUTTON;

        return MouseEvent(button, pos, globalPos, degreeDelta);
    }
}
