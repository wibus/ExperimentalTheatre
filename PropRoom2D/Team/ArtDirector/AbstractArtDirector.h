#ifndef PROPROOM2D_ABSTRACTARTDIRECTOR_H
#define PROPROOM2D_ABSTRACTARTDIRECTOR_H

#include <memory>

#include <CellarWorkbench/Camera/Camera.h>
#include <CellarWorkbench/DesignPattern/SpecificObserver.h>

#include "../../libPropRoom2D_global.h"


namespace prop2
{
    class Circle;
    class Polygon;
    class TextHud;
    class ImageHud;

    class PROP2D_EXPORT AbstractArtDirector :
            public cellar::SpecificObserver<cellar::CameraMsg>
    {
    protected:
        AbstractArtDirector()
        {
            setCamera(std::shared_ptr<cellar::Camera>(new cellar::Camera()));
        }

    public:
        virtual ~AbstractArtDirector() {}

        virtual void setup() =0;
        virtual void reset() =0;
        virtual void draw(double dt) =0;
        virtual void update(double dt) =0;

        virtual void notify(cellar::CameraMsg &msg) = 0;

        virtual void resize(int width, int height);
        virtual const std::shared_ptr<cellar::Camera>& camera() const;
        virtual void setCamera(const std::shared_ptr<cellar::Camera>& camera);

        virtual void manageCircle(const std::shared_ptr<Circle>& circle) =0;
        virtual void managePolygon(const std::shared_ptr<Polygon>& polygon) =0;
        virtual void manageTextHud(const std::shared_ptr<TextHud>& text) =0;
        virtual void manageImageHud(const std::shared_ptr<ImageHud>& image) =0;

        virtual void unmanageCircle(const std::shared_ptr<Circle>& circle) =0;
        virtual void unmanagePolygon(const std::shared_ptr<Polygon>& polygon) =0;
        virtual void unmanageTextHud(const std::shared_ptr<TextHud>& text) =0;
        virtual void unmanageImageHud(const std::shared_ptr<ImageHud>& image) =0;


    private:
        std::shared_ptr<cellar::Camera> _camera;
    };



    // IMPLEMENTATION //
    inline void AbstractArtDirector::resize(int width, int height)
    {
        _camera->updateViewport(width, height);
    }

    inline const std::shared_ptr<cellar::Camera>& AbstractArtDirector::camera() const
    {
        return _camera;
    }

    inline void AbstractArtDirector::setCamera(const std::shared_ptr<cellar::Camera>& camera)
    {
        if(_camera)
            _camera->unregisterObserver(*this);

        _camera = camera;

        if(_camera)
            _camera->registerObserver(*this);
    }
}

#endif // PROPROOM2D_ABSTRACTARTDIRECTOR_H
