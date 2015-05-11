#ifndef SCAENA_MOUSE_MOVE_DISPATCHER_H
#define SCAENA_MOUSE_MOVE_DISPATCHER_H

#include "StageManager.h"
#include "../Event/MouseEvent.h"


namespace scaena
{
    class SCAENA_EXPORT MouseMoveDispatcher : public StageManager
    {
    public:
        MouseMoveDispatcher(const MouseEvent& event);

        virtual void visit(Character& character);
        bool eventConsumed() const;


    private :
        MouseEvent _event;
        bool _eventConsumed;
    };



    // IMPLEMENTATION //
    inline bool MouseMoveDispatcher::eventConsumed() const
    {
        return _eventConsumed;
    }
}

#endif // SCAENA_MOUSE_MOVE_DISPATCHER_H
