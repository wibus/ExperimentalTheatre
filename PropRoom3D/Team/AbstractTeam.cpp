#include "AbstractTeam.h"

#include <CellarWorkbench/Camera/Camera.h>

#include "../Scene/Scene.h"
#include "ArtDirector/AbstractArtDirector.h"
#include "Choreographer/AbstractChoreographer.h"
#include "Designer/AbstractDesigner.h"


namespace prop3
{
    AbstractTeam::AbstractTeam(AbstractDesigner*  propDesigner,
                               AbstractChoreographer* choreographer) :
        _scene(new Scene()),
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
            artDir->setup(_scene);
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
        _scene->updateTimeStamp();
        if(_choreographer.get() != nullptr)
            _choreographer->update(dt);
        for(auto& artDir : _artDirectors)
            artDir->update(dt);
    }

    void AbstractTeam::addArtDirector(
        const std::shared_ptr<AbstractArtDirector>& artDirector)
    {
        _artDirectors.push_back(artDirector);
    }

    void AbstractTeam::clearProps()
    {
        std::vector<std::shared_ptr<Prop>> props = _scene->props();
        for(auto prop : props)
        {
            deleteProp(prop);
        }
    }

    std::shared_ptr<Prop> AbstractTeam::createProp()
    {
        std::shared_ptr<Prop> prop = _propDesigner->createProp();

        _scene->addProp(prop);
        if(_choreographer.get() != nullptr)
            _choreographer->manageProp(prop);
        for(auto& artDir : _artDirectors)
            artDir->manageProp(prop);

        return prop;
    }

    void AbstractTeam::deleteProp(std::shared_ptr<Prop>& prop)
    {
        _scene->removeProp(prop);
        if(_choreographer.get() != nullptr)
            _choreographer->unmanageProp(prop);
        for(auto& artDir : _artDirectors)
            artDir->unmanageProp(prop);

        prop.reset();
    }
}

