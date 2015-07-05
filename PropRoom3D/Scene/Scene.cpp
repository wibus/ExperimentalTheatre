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


    bool Scene::read(const std::string& fileName)
    {
        SceneReader reader;
        reader.read(fileName, *this);
    }

    bool Scene::write(const std::string& fileName) const
    {
        SceneWriter writer;
        writer.write(fileName, *this);
    }
}
