#include "DrawCaller.h"

#include "../../Play/Character.h"


namespace scaena
{
    DrawCaller::DrawCaller(const std::shared_ptr<View>& view, const StageTime& time) :
        _view(view),
        _time(time)
    {}

    void DrawCaller::visit(Character& character)
    {
        if(character.visible())
            character.draw(_view, _time);
    }
}
