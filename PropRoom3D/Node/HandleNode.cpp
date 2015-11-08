#include "HandleNode.h"


namespace prop3
{
    HandleNode::HandleNode(const std::string& name) :
        _isVisible(true),
        _name(name)
    {

    }

    HandleNode::~HandleNode()
    {

    }

    void HandleNode::setIsVisible(bool isVisible)
    {
        _isVisible = isVisible;

        stampCurrentUpdate();
    }

    void HandleNode::setName(const std::string& name)
    {
        _name = name;

        stampCurrentUpdate();
    }
}
