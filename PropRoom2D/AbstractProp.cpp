#include "AbstractProp.h"


namespace prop2
{
    PropId AbstractProp::_nextId_ = 0; //First assigned ID will be '0'

    AbstractProp::AbstractProp(PropType::Enum type) :
        _id(_assigneId_()),
        _propType(type),
        _isVisible(true)
    {
    }

    AbstractProp::~AbstractProp()
    {
    }
}
