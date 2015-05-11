#ifndef SCAENA_MOUSE_RELEASE_DISPATCHER_H
#define SCAENA_MOUSE_RELEASE_DISPATCHER_H

#include "StageManager.h"
#include "../Event/MouseEvent.h"


namespace scaena
{
    class SCAENA_EXPORT MouseReleaseDispatcher : public StageManager
    {
    public:
        MouseReleaseDispatcher(const MouseEvent& event);

        virtual void visit(Character& character);
        bool eventConsumed() const;


    private :
        MouseEvent _event;
        bool _eventConsumed;
    };



    // IMPLEMENTATION //
    inline bool MouseReleaseDispatcher::eventConsumed() const
    {
        return _eventConsumed;
    }
}

#endif // SCAENA_MOUSE_RELEASE_DISPATCHER_H
