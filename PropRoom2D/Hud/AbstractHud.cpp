#include "AbstractHud.h"


namespace prop2
{
    AbstractHud::AbstractHud(EPropType type) :
        AbstractProp(type),
        _horizontalAnchor(EHorizontalAnchor::LEFT),
        _verticalAnchor(EVerticalAnchor::BOTTOM)
    {
    }

    AbstractHud::~AbstractHud()
    {
    }
}

