#include "Scene.h"

#include <QFile>

#include <CellarWorkbench/Misc/Log.h>
#include <CellarWorkbench/Misc/StringUtils.h>

#include "SceneJsonReader.h"
#include "SceneJsonWriter.h"
#include "../Team/AbstractTeam.h"

using namespace std;
using namespace cellar;


namespace prop3
{
    Scene::Scene(const shared_ptr<AbstractTeam>& team) :
        _team(team)
    {

    }

    Scene::~Scene()
    {

    }

    bool Scene::load(const string& fileName, bool clearScene)
    {
        bool ok = false;
        string stream = fileToString(fileName, &ok);

        if(ok)
        {
            if(clearScene)
            {
                clearProps();
            }

            SceneJsonReader reader;
            reader.read(*this, stream);
        }
        else
        {
            getLog().postMessage(new Message('E', false,
                "Scene failed to load from '" + fileName + "'." +
                string(clearScene ? " Scene won't be cleared." : ""),
                "Scene"));
        }
    }

    bool Scene::save(const string& fileName, bool prettyPrint)
    {
        SceneJsonWriter writer;
        string stream = writer.write(*this, prettyPrint);

        if(!stream.empty() || _props.empty())
        {
            QFile file(fileName.c_str());
            file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
            file.write(stream.c_str());
            file.close();
        }
        else
        {
            getLog().postMessage(new Message('E', false,
                "Cannot save scene to '" + fileName + "'.",
                "Scene"));
        }
    }

    void Scene::deserialize(const std::string& stream, bool clearScene)
    {
        if(clearScene)
        {
            clearProps();
        }

        SceneJsonReader reader;
        reader.read(*this, stream);
    }

    std::string Scene::serialize()
    {
        SceneJsonWriter writer;
        return writer.write(*this, false /* pretty print */);
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

    void Scene::clearProps()
    {
        for(shared_ptr<Prop>& prop : _props)
        {
            _team->deleteProp(prop);
        }

        _props.clear();
    }

    shared_ptr<Prop> Scene::createProp()
    {
        shared_ptr<Prop> prop = _team->createProp();
        _props.push_back(prop);
        return prop;
    }
}
