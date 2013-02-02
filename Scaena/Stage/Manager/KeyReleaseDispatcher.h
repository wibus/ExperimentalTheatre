#ifndef SCAENA_KEY_RELEASE_DISPATCHER_H
#define SCAENA_KEY_RELEASE_DISPATCHER_H

#include "StageManager.h"
#include "Stage/Event/KeyboardEvent.h"


namespace scaena
{
    class SCAENA_EXPORT KeyReleaseDispatcher : public StageManager
    {
    public:
        KeyReleaseDispatcher(const KeyboardEvent& event);

        virtual void visit(AbstractCharacter& character);


    private :
        KeyboardEvent _event;
    };
}

#endif // SCAENA_KEY_RELEASE_DISPATCHER_H
