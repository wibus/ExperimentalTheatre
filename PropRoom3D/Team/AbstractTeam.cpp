#include "AbstractTeam.h"

#include <QFile>

#include <CellarWorkbench/Camera/Camera.h>
#include <CellarWorkbench/Misc/StringUtils.h>
#include <CellarWorkbench/Misc/Log.h>

#include "../StageSet/StageSet.h"
#include "../StageSet/StageSetJsonWriter.h"
#include "../StageSet/StageSetJsonReader.h"
#include "ArtDirector/AbstractArtDirector.h"
#include "Choreographer/AbstractChoreographer.h"
#include "Designer/AbstractDesigner.h"

using namespace std;
using namespace cellar;


namespace prop3
{
    AbstractTeam::AbstractTeam(AbstractDesigner*  propDesigner,
                               AbstractChoreographer* choreographer) :
        _stageSet(new StageSet()),
        _propDesigner(propDesigner),
        _choreographer(choreographer)
    {
    }

    AbstractTeam::~AbstractTeam()
    {
    }

    void AbstractTeam::setup()
    {
        _propDesigner->setup();
        if(_choreographer.get() != nullptr)
            _choreographer->setup();
        for(auto& artDir : _artDirectors)
            artDir->setup(_stageSet);

        auto env = _propDesigner->createEnvironment();

        _stageSet->setEnvironment(env);
        if(_choreographer.get() != nullptr)
            _choreographer->setEnvironment(env);
        for(auto& artDir : _artDirectors)
            artDir->setEnvironment(env);
    }

    void AbstractTeam::reset()
    {
        clearProps();

        _propDesigner->reset();
        if(_choreographer.get() != nullptr)
            _choreographer->reset();
        for(auto& artDir : _artDirectors)
            artDir->reset();
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
            const string& fileName,
            bool clearStageSet)
    {
        bool ok = false;
        string stream = fileToString(fileName, &ok);

        if(ok)
        {
            StageSetJsonReader reader;
            reader.deserialize(*this, stream, clearStageSet);
        }
        else
        {
            getLog().postMessage(new Message('E', false,
                "StageSet failed to load from '" + fileName + "'." +
                string(clearStageSet ? " StageSet won't be cleared." : ""),
                "AbstractTeam"));
        }
    }

    void AbstractTeam::addArtDirector(
        const shared_ptr<AbstractArtDirector>& artDirector)
    {
        _artDirectors.push_back(artDirector);
    }

    void AbstractTeam::clearProps()
    {
        auto props = _stageSet->props();
        for(auto prop : props)
        {
            deleteProp(prop);
        }
    }

    shared_ptr<Prop> AbstractTeam::createProp()
    {
        shared_ptr<Prop> prop = _propDesigner->createProp();

        _stageSet->addProp(prop);
        if(_choreographer.get() != nullptr)
            _choreographer->manageProp(prop);
        for(auto& artDir : _artDirectors)
            artDir->manageProp(prop);

        return prop;
    }

    void AbstractTeam::deleteProp(shared_ptr<Prop>& prop)
    {
        _stageSet->removeProp(prop);
        if(_choreographer.get() != nullptr)
            _choreographer->unmanageProp(prop);
        for(auto& artDir : _artDirectors)
            artDir->unmanageProp(prop);

        prop.reset();
    }
}

