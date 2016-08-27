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
        AbstractArtDirector();

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

        virtual void manageCircle(const std::shared_ptr<Circle>& circle);
        virtual void managePolygon(const std::shared_ptr<Polygon>& polygon);
        virtual void manageTextHud(const std::shared_ptr<TextHud>& text);
        virtual void manageImageHud(const std::shared_ptr<ImageHud>& image);

        virtual void unmanageCircle(const std::shared_ptr<Circle>& circle);
        virtual void unmanagePolygon(const std::shared_ptr<Polygon>& polygon);
        virtual void unmanageTextHud(const std::shared_ptr<TextHud>& text);
        virtual void unmanageImageHud(const std::shared_ptr<ImageHud>& image);


    protected:
        std::shared_ptr<cellar::Camera> _camera;        
        std::vector< std::shared_ptr<Circle> >  _circles;
        std::vector< std::shared_ptr<Polygon> > _polygons;
        std::vector< std::shared_ptr<TextHud> >  _texts;
        std::vector< std::shared_ptr<ImageHud> > _images;
    };



    // IMPLEMENTATION //
    inline const std::shared_ptr<cellar::Camera>& AbstractArtDirector::camera() const
    {
        return _camera;
    }

}

#endif // PROPROOM2D_ABSTRACTARTDIRECTOR_H
