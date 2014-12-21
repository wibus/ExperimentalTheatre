#ifndef SCAENA_KEY_PRESS_DISPATCHER_H
#define SCAENA_KEY_PRESS_DISPATCHER_H

#include "StageManager.h"
#include "../Event/KeyboardEvent.h"


namespace scaena
{
    class SCAENA_EXPORT KeyPressDispatcher : public StageManager
    {
    public:
        KeyPressDispatcher(const KeyboardEvent& event);

        virtual void visit(AbstractCharacter& character);


    private :
        KeyboardEvent _event;
    };
}

#endif // SCAENA_KEY_PRESS_DISPATCHER_H
