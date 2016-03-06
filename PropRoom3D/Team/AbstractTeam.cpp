#include "AbstractTeam.h"

#include <QFile>

#include <CellarWorkbench/Camera/Camera.h>
#include <CellarWorkbench/Misc/StringUtils.h>
#include <CellarWorkbench/Misc/Log.h>

#include "Node/StageSet.h"
#include "Serial/JsonWriter.h"
#include "Serial/JsonReader.h"
#include "ArtDirector/AbstractArtDirector.h"
#include "Choreographer/AbstractChoreographer.h"
using namespace std;
using namespace cellar;


namespace prop3
{
    AbstractTeam::AbstractTeam(const std::shared_ptr<AbstractChoreographer> &choreographer) :
        _stageSet(new StageSet()),
        _choreographer(choreographer),
        _artDirectors()
    {
    }

    AbstractTeam::~AbstractTeam()
    {
    }

    void AbstractTeam::setup()
    {
        for(auto& artDir : _artDirectors)
            artDir->setup(_stageSet);
        if(_choreographer.get() != nullptr)
            _choreographer->setup(_stageSet);
    }

    void AbstractTeam::terminate()
    {
        _stageSet->clear();

        if(_choreographer.get() != nullptr)
            _choreographer->terminate();
        for(auto& artDir : _artDirectors)
            artDir->terminate();
    }

    void AbstractTeam::update(double dt)
    {
        _stageSet->updateTimeStamp();

        if(_choreographer.get() != nullptr)
            _choreographer->update(dt);
        for(auto& artDir : _artDirectors)
            artDir->update(dt);
    }

    bool AbstractTeam::saveScene(const string& fileName, bool prettyPrint)
    {
        StageSetJsonWriter writer;
        string stream = writer.serialize(*_stageSet, prettyPrint);

        if(!stream.empty() || _stageSet->props().empty())
        {
            QFile file(fileName.c_str());
            file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
            file.write(stream.c_str());
            file.close();
        }
        else
        {
            getLog().postMessage(new Message('E', false,
                "Cannot save stageSet to '" + fileName + "'.",
                "AbstractTeam"));
            return false;
        }

        return true;
    }

    bool AbstractTeam::loadScene(
            const string& fileName)
    {
        bool ok = false;
        string stream = fileToString(fileName, &ok);

        if(ok)
        {
            StageSetJsonReader reader;
            reader.deserialize(*this, stream);
			return true;
        }
        else
        {
            getLog().postMessage(new Message('E', false,
                "StageSet failed to load from '" + fileName + "'.",
                "AbstractTeam"));
			return false;
        }
    }

    void AbstractTeam::addArtDirector(
        const shared_ptr<AbstractArtDirector>& artDirector)
    {
        _artDirectors.push_back(artDirector);
    }

    void AbstractTeam::switchChoreographer(
        const std::shared_ptr<AbstractChoreographer>& choreographer)
    {
        _choreographer = choreographer;
    }
}

