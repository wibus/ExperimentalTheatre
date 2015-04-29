#include "AbstractPropTeam.h"

#include <CellarWorkbench/Camera/Camera.h>

#include "AbstractPropDesigner.h"
#include "AbstractArtDirector.h"
#include "AbstractChoreographer.h"


namespace prop2
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

    std::shared_ptr<Circle> AbstractPropTeam::createCircle()
    {
        std::shared_ptr<Circle> circle = _propDesigner->createCircle();

        _artDirector->manageCircle(circle);
        _choreographer->manageCircle(circle);

        return circle;
    }

    std::shared_ptr<Polygon> AbstractPropTeam::createPolygon(int nbVertices)
    {
        std::shared_ptr<Polygon> polygon = _propDesigner->createPolygon(nbVertices);

        _artDirector->managePolygon(polygon);
        _choreographer->managePolygon(polygon);

        return polygon;
    }

    std::shared_ptr<TextHud> AbstractPropTeam::createTextHud()
    {
        std::shared_ptr<TextHud> textHud = _propDesigner->createTextHud();

        _artDirector->manageTextHud(textHud);
        _choreographer->manageTextHud(textHud);

        return textHud;
    }

    std::shared_ptr<ImageHud> AbstractPropTeam::createImageHud()
    {
        std::shared_ptr<ImageHud> imageHud = _propDesigner->createImageHud();

        _artDirector->manageImageHud(imageHud);
        _choreographer->manageImageHud(imageHud);

        return imageHud;
    }

    void AbstractPropTeam::deleteCircle(std::shared_ptr<Circle>& circle)
    {
        _artDirector->unmanageCircle(circle);
        _choreographer->unmanageCircle(circle);
        circle.reset();
    }

    void AbstractPropTeam::deletePolygon(std::shared_ptr<Polygon>& polygon)
    {
        _artDirector->unmanagePolygon(polygon);
        _choreographer->unmanagePolygon(polygon);
        polygon.reset();
    }

    void AbstractPropTeam::deleteTextHud(std::shared_ptr<TextHud>& textHud)
    {
        _artDirector->unmanageTextHud(textHud);
        _choreographer->unmanageTextHud(textHud);
        textHud.reset();
    }

    void AbstractPropTeam::deleteImageHud(std::shared_ptr<ImageHud>& imageHud)
    {
        _artDirector->unmanageImageHud(imageHud);
        _choreographer->unmanageImageHud(imageHud);
        imageHud.reset();
    }

    void AbstractPropTeam::resize(int width, int height)
    {
        _artDirector->resize(width, height);
    }

    std::shared_ptr<cellar::Camera> AbstractPropTeam::camera() const
    {
        return _artDirector->camera();
    }

    void AbstractPropTeam::setCamera(const std::shared_ptr<cellar::Camera>& camera)
    {
        _artDirector->setCamera(camera);
    }

    void AbstractPropTeam::setGravity(const glm::dvec2& unitsPerSecondSquared)
    {
        _choreographer->setGravity(unitsPerSecondSquared);
    }
}

