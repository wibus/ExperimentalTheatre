#ifndef SCAENA_MOUSE_EVENT_H
#define SCAENA_MOUSE_EVENT_H

#include <CellarWorkbench/DataStructure/Vector.h>

#include "../../libScaena_global.h"
#include "EventTypes.h"


namespace scaena
{
    class SCAENA_EXPORT MouseEvent
    {
    public:
        MouseEvent(EMouseButton button,
                   const cellar::Vec2i& pos,
                   const cellar::Vec2i& globalPos);

        EMouseButton button() const;
        cellar::Vec2i position() const;
        cellar::Vec2i globalPosition() const;

    private:
        EMouseButton _button;
        cellar::Vec2i _position;
        cellar::Vec2i _globalPosition;
    };



    // IMPLEMENTATION //
    inline EMouseButton MouseEvent::button() const
        {return _button;}

    inline cellar::Vec2i MouseEvent::position() const
        {return _position;}

    inline cellar::Vec2i MouseEvent::globalPosition() const
        {return _globalPosition;}
}

#endif // SCAENA_MOUSE_EVENT_H
