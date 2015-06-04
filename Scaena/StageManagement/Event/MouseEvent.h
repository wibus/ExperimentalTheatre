#ifndef SCAENA_MOUSE_EVENT_H
#define SCAENA_MOUSE_EVENT_H

#include <GLM/glm.hpp>

#include "../../libScaena_global.h"
#include "EventTypes.h"

class QMouseEvent;
class QWheelEvent;


namespace scaena
{
    class SCAENA_EXPORT MouseEvent
    {
    public:
        MouseEvent(EMouseButton button,
                   const glm::ivec2& pos,
                   const glm::ivec2& globalPos,
                   int degreeDelta);

        EMouseButton button() const;
        glm::ivec2 position() const;
        glm::ivec2 globalPosition() const;
        int degreeDelta() const;

        static MouseEvent convertQMouseEvent(QMouseEvent* event);
        static MouseEvent convertQWheelEvent(QWheelEvent* event);

    private:
        EMouseButton _button;
        glm::ivec2 _position;
        glm::ivec2 _globalPosition;
        int _degreeDelta;
    };



    // IMPLEMENTATION //
    inline EMouseButton MouseEvent::button() const
        {return _button;}

    inline glm::ivec2 MouseEvent::position() const
        {return _position;}

    inline glm::ivec2 MouseEvent::globalPosition() const
        {return _globalPosition;}

    inline int MouseEvent::degreeDelta() const
        {return _degreeDelta;}
}

#endif // SCAENA_MOUSE_EVENT_H
