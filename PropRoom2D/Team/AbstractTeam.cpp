#include "AbstractTeam.h"

#include <CellarWorkbench/Camera/Camera.h>

#include "ArtDirector/AbstractArtDirector.h"
#include "Choreographer/AbstractChoreographer.h"
#include "Designer/AbstractDesigner.h"


namespace prop2
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

    void AbstractTeam::addArtDirector(const std::shared_ptr<AbstractArtDirector>& artDirector)
    {
        _artDirectors.push_back(artDirector);
    }

    std::shared_ptr<Circle> AbstractTeam::createCircle()
    {
        std::shared_ptr<Circle> circle = _propDesigner->createCircle();

        _choreographer->manageCircle(circle);
        for(auto& artDir : _artDirectors)
        {
            artDir->manageCircle(circle);
        }

        return circle;
    }

    std::shared_ptr<Polygon> AbstractTeam::createPolygon(int nbVertices)
    {
        std::shared_ptr<Polygon> polygon = _propDesigner->createPolygon(nbVertices);

        _choreographer->managePolygon(polygon);
        for(auto& artDir : _artDirectors)
        {
            artDir->managePolygon(polygon);
        }

        return polygon;
    }

    std::shared_ptr<TextHud> AbstractTeam::createTextHud()
    {
        std::shared_ptr<TextHud> textHud = _propDesigner->createTextHud();

        _choreographer->manageTextHud(textHud);
        for(auto& artDir : _artDirectors)
        {
            artDir->manageTextHud(textHud);
        }

        return textHud;
    }

    std::shared_ptr<ImageHud> AbstractTeam::createImageHud()
    {
        std::shared_ptr<ImageHud> imageHud = _propDesigner->createImageHud();

        _choreographer->manageImageHud(imageHud);
        for(auto& artDir : _artDirectors)
        {
            artDir->manageImageHud(imageHud);
        }

        return imageHud;
    }

    void AbstractTeam::deleteCircle(std::shared_ptr<Circle>& circle)
    {
        _choreographer->unmanageCircle(circle);
        for(auto& artDir : _artDirectors)
        {
            artDir->unmanageCircle(circle);
        }
        circle.reset();
    }

    void AbstractTeam::deletePolygon(std::shared_ptr<Polygon>& polygon)
    {
        _choreographer->unmanagePolygon(polygon);
        for(auto& artDir : _artDirectors)
        {
            artDir->unmanagePolygon(polygon);
        }
        polygon.reset();
    }

    void AbstractTeam::deleteTextHud(std::shared_ptr<TextHud>& textHud)
    {
        _choreographer->unmanageTextHud(textHud);
        for(auto& artDir : _artDirectors)
        {
            artDir->unmanageTextHud(textHud);
        }
        textHud.reset();
    }

    void AbstractTeam::deleteImageHud(std::shared_ptr<ImageHud>& imageHud)
    {
        _choreographer->unmanageImageHud(imageHud);
        for(auto& artDir : _artDirectors)
        {
            artDir->unmanageImageHud(imageHud);
        }
        imageHud.reset();
    }

    void AbstractTeam::setGravity(const glm::dvec2& unitsPerSecondSquared)
    {
        _choreographer->setGravity(unitsPerSecondSquared);
    }
}

