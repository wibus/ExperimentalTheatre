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

    std::shared_ptr<Brep> AbstractPropTeam::createBrep()
    {
        std::shared_ptr<Brep> brep = _propDesigner->createBrep();

        _artDirector->manageBrep(brep);
        _choreographer->manageBrep(brep);

        return brep;
    }

    std::shared_ptr<Mesh> AbstractPropTeam::createMesh(int vertexCount)
    {
        std::shared_ptr<Mesh> mesh = _propDesigner->createMesh(vertexCount);

        _artDirector->manageMesh(mesh);
        _choreographer->manageMesh(mesh);

        return mesh;
    }

    void AbstractPropTeam::deleteBrep(std::shared_ptr<Brep>& brep)
    {
        _artDirector->unmanageBrep(brep);
        _choreographer->unmanageBrep(brep);
        brep.reset();
    }

    void AbstractPropTeam::deleteMesh(std::shared_ptr<Mesh>& mesh)
    {
        _artDirector->unmanageMesh(mesh);
        _choreographer->unmanageMesh(mesh);
        mesh.reset();
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

