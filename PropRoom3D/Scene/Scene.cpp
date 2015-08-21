#include "Scene.h"

#include <cmath>

using namespace std;


namespace prop3
{
    Scene::Scene() :
        _props(),
        _timeStamp(),
        _sceneChanged(false)
    {

    }

    Scene::~Scene()
    {

    }

    void Scene::makeTraveling(SceneVisitor& visitor)
    {
        vector<shared_ptr<SceneNode>> nodeStack;

        // Level-order tree insertion
        nodeStack.insert(nodeStack.begin(), _props.begin(), _props.end());
        for(size_t i=0; i < nodeStack.size(); ++i)
        {
            shared_ptr<SceneNode> node = nodeStack[i];

            if(node.get() != nullptr)
            {
                vector<shared_ptr<SceneNode>> children = node->children();
                nodeStack.insert(nodeStack.end(), children.begin(), children.end());
            }
        }

        // Post-order tree visit
        while(!nodeStack.empty())
        {
            shared_ptr<SceneNode> node = nodeStack.back();

            if(node.get() != nullptr)
            {
                node->accept(visitor);
            }

            nodeStack.pop_back();
        }
    }

    void Scene::addProp(const std::shared_ptr<Prop>& prop)
    {
        _props.push_back(prop);
    }

    void Scene::removeProp(const std::shared_ptr<Prop>& prop)
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

    bool Scene::updateTimeStamp()
    {
        TimeStamp lastTimeStamp = _timeStamp;

        vector<shared_ptr<SceneNode>> nodes;
        nodes.insert(nodes.begin(), _props.begin(), _props.end());
        for(size_t i=0; i < nodes.size(); ++i)
        {
            shared_ptr<SceneNode> node = nodes[i];

            if(node.get() != nullptr)
            {
                vector<shared_ptr<SceneNode>> children = node->children();
                nodes.insert(nodes.end(), children.begin(), children.end());
                _timeStamp = std::max(_timeStamp, node->timeStamp());
            }
        }

        _sceneChanged = lastTimeStamp < _timeStamp;
        return _sceneChanged;
    }
}
