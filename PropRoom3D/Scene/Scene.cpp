#include "Scene.h"

#include "SceneReader.h"
#include "SceneWriter.h"


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
                      const std::shared_ptr<AbstractTeam>& team) const
    {
        SceneWriter writer;
        writer.write(fileName, team, *this);
    }
}
