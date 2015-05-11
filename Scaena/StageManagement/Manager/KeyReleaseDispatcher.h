#ifndef SCAENA_KEY_RELEASE_DISPATCHER_H
#define SCAENA_KEY_RELEASE_DISPATCHER_H

#include "StageManager.h"
#include "../Event/KeyboardEvent.h"


namespace scaena
{
    class SCAENA_EXPORT KeyReleaseDispatcher : public StageManager
    {
    public:
        KeyReleaseDispatcher(const KeyboardEvent& event);

        virtual void visit(Character& character);
        bool eventConsumed() const;


    private :
        KeyboardEvent _event;
        bool _eventConsumed;
    };



    // IMPLEMENTATION //
    inline bool KeyReleaseDispatcher::eventConsumed() const
    {
        return _eventConsumed;
    }
}

#endif // SCAENA_KEY_RELEASE_DISPATCHER_H
