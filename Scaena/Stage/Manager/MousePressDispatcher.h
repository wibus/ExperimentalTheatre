#ifndef SCAENA_MOUSE_PRESS_DISPATCHER_H
#define SCAENA_MOUSE_PRESS_DISPATCHER_H

#include "StageManager.h"
#include "Stage/Event/MouseEvent.h"


namespace scaena
{
    class SCAENA_EXPORT MousePressDispatcher : public StageManager
    {
    public:
        MousePressDispatcher(const MouseEvent& event);

        virtual void visit(AbstractCharacter& character);


    private :
        MouseEvent _event;
    };
}

#endif // SCAENA_MOUSE_PRESS_DISPATCHER_H
