#include "AbstractHud.h"


namespace prop2
{
    AbstractHud::AbstractHud(PropType::Enum type) :
        AbstractProp(type),
        _horizontalAnchor(HorizontalAnchor::LEFT),
        _verticalAnchor(VerticalAnchor::BOTTOM)
    {
    }

    AbstractHud::~AbstractHud()
    {
    }
}

