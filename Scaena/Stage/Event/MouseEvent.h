#ifndef SCAENA_MOUSE_EVENT_H
#define SCAENA_MOUSE_EVENT_H

#include <DataStructure/Vector2D.h>

#include "libScaena_global.h"
#include "EventTypes.h"


namespace scaena
{
    class SCAENA_EXPORT MouseEvent
    {
    public:
        MouseEvent(Mouse::Button button,
                   const cellar::Vector2D<int>& pos,
                   const cellar::Vector2D<int>& globalPos);

        Mouse::Button button() const;
        cellar::Vector2D<int> position() const;
        cellar::Vector2D<int> globalPosition() const;

    private:
        Mouse::Button _button;
        cellar::Vector2D<int> _position;
        cellar::Vector2D<int> _globalPosition;
    };



    // IMPLEMENTATION //
    inline Mouse::Button MouseEvent::button() const
        {return _button;}

    inline cellar::Vector2D<int> MouseEvent::position() const
        {return _position;}

    inline cellar::Vector2D<int> MouseEvent::globalPosition() const
        {return _globalPosition;}
}

#endif // SCAENA_MOUSE_EVENT_H
