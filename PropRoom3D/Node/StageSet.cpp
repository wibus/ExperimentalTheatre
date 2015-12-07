#include "StageSet.h"

#include <algorithm>

#include "Node/Visitor.h"
#include "Light/Environment.h"

using namespace std;


namespace prop3
{
    StageSet::StageSet() :
        StageZone("Stage set"),
        _environment(),
        _stageSetChanged(false),
        _lastTimeStamp()
    {
    }

    StageSet::~StageSet()
    {

    }

    void StageSet::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<Node>> StageSet::children() const
    {
        std::vector<std::shared_ptr<Node>> c = StageZone::children();
        c.push_back(_environment);
        return c;
    }
    void StageSet::setEnvironment(const std::shared_ptr<Environment>& env)
    {
        _environment = env;

        stampCurrentUpdate();
    }

    void StageSet::makeTraveling(Visitor& visitor)
    {
        vector<Node*> nodeStack;

        // Level-order tree insertion
        nodeStack.push_back(this);
        for(size_t i=0; i < nodeStack.size(); ++i)
        {
            Node* node = nodeStack[i];

            if(node != nullptr)
            {
                for(const auto& n : node->children())
                    nodeStack.push_back(n.get());
            }
        }

        // Post-order tree visit
        while(!nodeStack.empty())
        {
            Node* node = nodeStack.back();

            if(node != nullptr)
            {
                node->accept(visitor);
            }

            nodeStack.pop_back();
        }
    }

    bool StageSet::updateTimeStamp()
    {
        TimeStamp maxTimeStamp = timeStamp();

        vector<shared_ptr<Node>> nodes = children();
        for(size_t i=0; i < nodes.size(); ++i)
        {
            shared_ptr<Node> node = nodes[i];

            if(node.get() != nullptr)
            {
                vector<shared_ptr<Node>> children = node->children();
                nodes.insert(nodes.end(), children.begin(), children.end());
                maxTimeStamp = std::max(maxTimeStamp, node->timeStamp());
            }
        }

        _stageSetChanged = _lastTimeStamp < maxTimeStamp;
        _lastTimeStamp = maxTimeStamp;
        return _stageSetChanged;
    }
}
