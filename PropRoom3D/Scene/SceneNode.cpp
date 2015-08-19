#include "SceneNode.h"


namespace prop3
{
    SceneNode::SceneNode()
    {

    }

    SceneNode::~SceneNode()
    {

    }

    std::vector<std::shared_ptr<SceneNode>> SceneNode::children() const
    {
        return std::vector<std::shared_ptr<SceneNode>>();
    }
}
