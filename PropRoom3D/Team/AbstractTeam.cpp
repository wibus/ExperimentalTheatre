#include "AbstractTeam.h"

#include <CellarWorkbench/Camera/Camera.h>

#include "ArtDirector/AbstractArtDirector.h"
#include "Choreographer/AbstractChoreographer.h"
#include "Designer/AbstractDesigner.h"


namespace prop3
{
    AbstractTeam::AbstractTeam(AbstractDesigner*  propDesigner,
                               AbstractChoreographer* choreographer) :
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
        _choreographer->setup();
        for(auto& artDir : _artDirectors)
        {
            artDir->setup();
        }
    }

    void AbstractTeam::reset()
    {
        _propDesigner->reset();
        _choreographer->reset();
        for(auto& artDir : _artDirectors)
        {
            artDir->reset();
        }
    }

    void AbstractTeam::update(double dt)
    {
        _choreographer->update(dt);
        for(auto& artDir : _artDirectors)
        {
            artDir->update(dt);
        }
    }

    void AbstractTeam::addArtDirector(
        const std::shared_ptr<AbstractArtDirector>& artDirector)
    {
        _artDirectors.push_back(artDirector);
    }

    std::shared_ptr<Prop> AbstractTeam::createProp()
    {
        std::shared_ptr<Prop> prop = _propDesigner->createProp();

        _choreographer->manageProp(prop);
        for(auto& artDir : _artDirectors)
        {
            artDir->manageProp(prop);
        }

        return prop;
    }

    void AbstractTeam::deleteProp(std::shared_ptr<Prop>& prop)
    {
        _choreographer->unmanageProp(prop);
        for(auto& artDir : _artDirectors)
        {
            artDir->unmanageProp(prop);
        }
        prop.reset();
    }
}

