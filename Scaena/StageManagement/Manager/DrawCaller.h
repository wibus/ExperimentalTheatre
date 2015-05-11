#ifndef SCAENA_DRAW_CALLER_H
#define SCAENA_DRAW_CALLER_H

#include <memory>

#include "StageManager.h"
#include "../Event/StageTime.h"


namespace scaena
{
    class View;


    class SCAENA_EXPORT DrawCaller : public StageManager
    {
    public:
        DrawCaller(const std::shared_ptr<View>& view, const StageTime& time);

        virtual void visit(Character& character);


    private :
        std::shared_ptr<View> _view;
        StageTime _time;
    };
}

#endif // SCAENA_DRAW_CALLER_H
