#include "AbstractArtDirector.h"

#include "../../Prop/Shape/Circle.h"
#include "../../Prop/Shape/Polygon.h"
#include "../../Prop/Hud/TextHud.h"
#include "../../Prop/Hud/ImageHud.h"


namespace prop2
{
    AbstractArtDirector::AbstractArtDirector() :
        _circles(),
        _polygons(),
        _texts(),
        _images()
    {
        setCamera(std::shared_ptr<cellar::Camera>(new cellar::Camera()));
    }

    void AbstractArtDirector::resize(int width, int height)
    {
        _camera->updateViewport(width, height);
    }

    void AbstractArtDirector::setCamera(const std::shared_ptr<cellar::Camera>& camera)
    {
        if(_camera)
            _camera->unregisterObserver(*this);

        _camera = camera;

        if(_camera)
            _camera->registerObserver(*this);
    }

    void AbstractArtDirector::manageCircle(const std::shared_ptr<Circle>& circle)
    {
        _circles.push_back(circle);
    }

    void AbstractArtDirector::managePolygon(const std::shared_ptr<Polygon>& polygon)
    {
        _polygons.push_back(polygon);
    }

    void AbstractArtDirector::manageTextHud(const std::shared_ptr<TextHud>& text)
    {
        _texts.push_back(text);
    }

    void AbstractArtDirector::manageImageHud(const std::shared_ptr<ImageHud>& image)
    {
        _images.push_back(image);
    }

    void AbstractArtDirector::unmanageCircle(const std::shared_ptr<Circle>& circle)
    {
        for(auto it = _circles.begin(); it != _circles.end(); ++it)
        {
            if((*it)->id() == circle->id())
            {
                _circles.erase(it);
                return;
            }
        }
    }

    void AbstractArtDirector::unmanagePolygon(const std::shared_ptr<Polygon>& polygon)
    {
        for(auto it = _polygons.begin(); it != _polygons.end(); ++it)
        {
            if((*it)->id() == polygon->id())
            {
                _polygons.erase(it);
                return;
            }
        }
    }

    void AbstractArtDirector::unmanageTextHud(const std::shared_ptr<TextHud>& text)
    {
        for(auto it = _texts.begin(); it != _texts.end(); ++it)
        {
            if((*it)->id() == text->id())
            {
                _texts.erase(it);
                return;
            }
        }
    }

    void AbstractArtDirector::unmanageImageHud(const std::shared_ptr<ImageHud>& image)
    {
        for(auto it = _images.begin(); it != _images.end(); ++it)
        {
            if((*it)->id() == image->id())
            {
                _images.erase(it);
                return;
            }
        }
    }
}
