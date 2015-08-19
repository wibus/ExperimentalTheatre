#include "Scene.h"

#include "SceneReader.h"
#include "SceneWriter.h"
#include "../Team/AbstractTeam.h"


namespace prop3
{
    Scene::Scene()
    {

    }

    Scene::~Scene()
    {

    }

    bool Scene::read(const std::string& fileName,
                     const std::shared_ptr<AbstractTeam>& team)
    {
        SceneReader reader;
        reader.read(fileName, team, *this);
    }

    bool Scene::write(const std::string& fileName,
                      const std::shared_ptr<AbstractTeam>& team)
    {
        SceneWriter writer;
        writer.write(fileName, team, *this);
    }

    void Scene::makeTraveling(SceneVisitor& visitor)
    {
        std::vector<std::shared_ptr<SceneNode>> nodeStack;

        // Level-order tree insertion
        nodeStack.insert(nodeStack.begin(), _props.begin(), _props.end());
        for(size_t i=0; i < nodeStack.size(); ++i)
        {
            std::shared_ptr<SceneNode> node = nodeStack[i];

            if(node.get() != nullptr)
            {
                std::vector<std::shared_ptr<SceneNode>> children = node->children();
                nodeStack.insert(nodeStack.end(), children.begin(), children.end());
            }
        }

        // Post-order tree visit
        while(!nodeStack.empty())
        {
            std::shared_ptr<SceneNode> node = nodeStack.back();

            if(node.get() != nullptr)
            {
                node->accept(visitor);
            }

            nodeStack.pop_back();
        }
    }

    void Scene::clearProps(const std::shared_ptr<AbstractTeam>& team)
    {
        for(std::shared_ptr<Prop>& prop : _props)
        {
            team->deleteProp(prop);
        }

        _props.clear();
    }

    std::shared_ptr<Prop> Scene::addProp(const std::shared_ptr<AbstractTeam>& team)
    {
        std::shared_ptr<Prop> prop = team->createProp();
        _props.push_back(prop);
        return prop;
    }
}
