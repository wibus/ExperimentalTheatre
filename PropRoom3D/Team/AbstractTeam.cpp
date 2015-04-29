#include "AbstractTeam.h"

#include <CellarWorkbench/Camera/Camera.h>

#include "ArtDirector/AbstractArtDirector.h"
#include "Choreographer/AbstractChoreographer.h"
#include "Designer/AbstractDesigner.h"


namespace prop3
{
    AbstractTeam::AbstractTeam(AbstractDesigner*  propDesigner,
                                       AbstractArtDirector*   artDirector,
                                       AbstractChoreographer* choreographer) :
        _propDesigner(propDesigner),
        _artDirector(artDirector),
        _choreographer(choreographer)
    {
    }

    AbstractTeam::~AbstractTeam()
    {
    }

    void AbstractTeam::setup()
    {
        _propDesigner->setup();
        _artDirector->setup();
        _choreographer->setup();
    }

    void AbstractTeam::reset()
    {
        _propDesigner->reset();
        _artDirector->reset();
        _choreographer->reset();
    }

    void AbstractTeam::update(double dt)
    {
        _choreographer->update(dt);
    }

    void AbstractTeam::draw(double dt)
    {
        _artDirector->draw(dt);
    }

    std::shared_ptr<Prop> AbstractTeam::createProp()
    {
        std::shared_ptr<Prop> prop = _propDesigner->createProp();

        _artDirector->manageProp(prop);
        _choreographer->manageProp(prop);

        return prop;
    }

    void AbstractTeam::deleteProp(std::shared_ptr<Prop>& prop)
    {
        _artDirector->unmanageProp(prop);
        _choreographer->unmanageProp(prop);
        prop.reset();
    }

    void AbstractTeam::setCamera(cellar::Camera& camera)
    {
        camera.registerObserver(*_artDirector);
    }

    void AbstractTeam::setGravity(const glm::dvec3& unitsPerSecondSquared)
    {
        _choreographer->setGravity(unitsPerSecondSquared);
    }
}

