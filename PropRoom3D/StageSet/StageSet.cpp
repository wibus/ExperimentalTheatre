#include "StageSet.h"

#include <cmath>

using namespace std;


namespace prop3
{
    StageSet::StageSet() :
        _props(),
        _timeStamp(),
        _stageSetChanged(false)
    {

    }

    StageSet::~StageSet()
    {

    }

    void StageSet::makeTraveling(StageSetVisitor& visitor)
    {
        vector<shared_ptr<StageSetNode>> nodeStack;

        // Level-order tree insertion
        nodeStack.push_back(_environment);
        nodeStack.insert(nodeStack.begin(), _props.begin(), _props.end());
        for(size_t i=0; i < nodeStack.size(); ++i)
        {
            shared_ptr<StageSetNode> node = nodeStack[i];

            if(node.get() != nullptr)
            {
                vector<shared_ptr<StageSetNode>> children = node->children();
                nodeStack.insert(nodeStack.end(), children.begin(), children.end());
            }
        }

        // Post-order tree visit
        while(!nodeStack.empty())
        {
            shared_ptr<StageSetNode> node = nodeStack.back();

            if(node.get() != nullptr)
            {
                node->accept(visitor);
            }

            nodeStack.pop_back();
        }
    }

    void StageSet::addProp(const std::shared_ptr<Prop>& prop)
    {
        _props.push_back(prop);
    }

    void StageSet::removeProp(const std::shared_ptr<Prop>& prop)
    {
        auto propIt = _props.begin();
        while(propIt != _props.end())
        {
            if(propIt->get() == prop.get())
            {
                _props.erase(propIt);
                return;
            }
        }
    }

    void StageSet::setEnvironment(const std::shared_ptr<Environment>& env)
    {
        _environment = env;
    }

    bool StageSet::updateTimeStamp()
    {
        TimeStamp lastTimeStamp = _timeStamp;

        vector<shared_ptr<StageSetNode>> nodes;

        nodes.push_back(_environment);
        nodes.insert(nodes.begin(), _props.begin(), _props.end());
        for(size_t i=0; i < nodes.size(); ++i)
        {
            shared_ptr<StageSetNode> node = nodes[i];

            if(node.get() != nullptr)
            {
                vector<shared_ptr<StageSetNode>> children = node->children();
                nodes.insert(nodes.end(), children.begin(), children.end());
                _timeStamp = std::max(_timeStamp, node->timeStamp());
            }
        }

        _stageSetChanged = lastTimeStamp < _timeStamp;
        return _stageSetChanged;
    }
}
