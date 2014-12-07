#include "AbstractProp.h"


namespace prop3
{
    PropId AbstractProp::_nextId_ = 0; //First assigned ID will be '0'

    AbstractProp::AbstractProp(EPropType type) :
        _id(_assigneId_()),
        _propType(type),
        _isVisible(true)
    {
    }

    AbstractProp::~AbstractProp()
    {
    }
}
