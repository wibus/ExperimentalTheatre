#ifndef SCAENA_MOUSE_RELEASE_DISPATCHER_H
#define SCAENA_MOUSE_RELEASE_DISPATCHER_H

#include "StageManager.h"
#include "Stage/Event/MouseEvent.h"


namespace scaena
{
    class SCAENA_EXPORT MouseReleaseDispatcher : public StageManager
    {
    public:
        MouseReleaseDispatcher(const MouseEvent& event);

        virtual void visit(AbstractCharacter& character);


    private :
        MouseEvent _event;
    };
}

#endif // SCAENA_MOUSE_RELEASE_DISPATCHER_H
