#ifndef SCAENA_MOUSE_EVENT_H
#define SCAENA_MOUSE_EVENT_H

#include <GLM/glm.hpp>

#include "../../libScaena_global.h"
#include "EventTypes.h"

class QMouseEvent;


namespace scaena
{
    class SCAENA_EXPORT MouseEvent
    {
    public:
        MouseEvent(EMouseButton button,
                   const glm::ivec2& pos,
                   const glm::ivec2& globalPos);

        EMouseButton button() const;
        glm::ivec2 position() const;
        glm::ivec2 globalPosition() const;

        static MouseEvent convertQMouseEvent(QMouseEvent* event);

    private:
        EMouseButton _button;
        glm::ivec2 _position;
        glm::ivec2 _globalPosition;
    };



    // IMPLEMENTATION //
    inline EMouseButton MouseEvent::button() const
        {return _button;}

    inline glm::ivec2 MouseEvent::position() const
        {return _position;}

    inline glm::ivec2 MouseEvent::globalPosition() const
        {return _globalPosition;}
}

#endif // SCAENA_MOUSE_EVENT_H
