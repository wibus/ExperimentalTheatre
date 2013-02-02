#ifndef SCAENA_MOUSE_MOVE_DISPATCHER_H
#define SCAENA_MOUSE_MOVE_DISPATCHER_H

#include "StageManager.h"
#include "Stage/Event/MouseEvent.h"


namespace scaena
{
    class SCAENA_EXPORT MouseMoveDispatcher : public StageManager
    {
    public:
        MouseMoveDispatcher(const MouseEvent& event);

        virtual void visit(AbstractCharacter& character);


    private :
        MouseEvent _event;
    };
}

#endif // SCAENA_MOUSE_MOVE_DISPATCHER_H
