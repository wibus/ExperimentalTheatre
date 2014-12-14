#include "AbstractPropTeam.h"
#include "AbstractPropDesigner.h"
#include "AbstractArtDirector.h"
#include "AbstractChoreographer.h"

#include <Camera/Camera.h>


namespace prop3
{
    AbstractPropTeam::AbstractPropTeam(AbstractPropDesigner*  propDesigner,
                                       AbstractArtDirector*   artDirector,
                                       AbstractChoreographer* choreographer) :
        _propDesigner(propDesigner),
        _artDirector(artDirector),
        _choreographer(choreographer)
    {
    }

    AbstractPropTeam::~AbstractPropTeam()
    {
    }

    void AbstractPropTeam::setup()
    {
        _propDesigner->setup();
        _artDirector->setup();
        _choreographer->setup();
    }

    void AbstractPropTeam::reset()
    {
        _propDesigner->reset();
        _artDirector->reset();
        _choreographer->reset();
    }

    void AbstractPropTeam::update(double dt)
    {
        _choreographer->update(dt);
    }

    void AbstractPropTeam::draw(double dt)
    {
        _artDirector->draw(dt);
    }

    std::shared_ptr<Prop> AbstractPropTeam::createProp()
    {
        std::shared_ptr<Prop> prop = _propDesigner->createProp();

        _artDirector->manageProp(prop);
        _choreographer->manageProp(prop);

        return prop;
    }

    void AbstractPropTeam::deleteProp(std::shared_ptr<Prop>& prop)
    {
        _artDirector->unmanageProp(prop);
        _choreographer->unmanageProp(prop);
        prop.reset();
    }

    void AbstractPropTeam::setCamera(media::Camera& camera)
    {
        camera.registerObserver(*_artDirector);
        camera.refresh();
    }

    void AbstractPropTeam::setGravity(const glm::dvec3& unitsPerSecondSquared)
    {
        _choreographer->setGravity(unitsPerSecondSquared);
    }
}

