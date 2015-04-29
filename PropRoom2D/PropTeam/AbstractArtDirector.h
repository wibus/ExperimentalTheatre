#ifndef PROPROOM2D_ABSTRACTARTDIRECTOR_H
#define PROPROOM2D_ABSTRACTARTDIRECTOR_H

#include <memory>

#include <CellarWorkbench/DesignPattern/SpecificObserver.h>

#include "AbstractTeamMember.h"


namespace cellar
{
    class Camera;
    class CameraMsg;
}

namespace prop2
{
    class Circle;
    class Polygon;
    class TextHud;
    class ImageHud;

    class PROP2D_EXPORT AbstractArtDirector :
            public cellar::SpecificObserver<cellar::CameraMsg>,
            public AbstractTeamMember
    {
    protected:
        AbstractArtDirector() {}

    public:
        virtual ~AbstractArtDirector() {}

        virtual void resize(int width, int height) = 0;
        virtual void notify(cellar::CameraMsg &msg) = 0;
        virtual std::shared_ptr<cellar::Camera> camera() const = 0;
        virtual void setCamera(const std::shared_ptr<cellar::Camera>& camera) = 0;

        virtual void draw(double dt) =0;

        virtual void manageCircle(const std::shared_ptr<Circle>& circle) =0;
        virtual void managePolygon(const std::shared_ptr<Polygon>& polygon) =0;
        virtual void manageTextHud(const std::shared_ptr<TextHud>& text) =0;
        virtual void manageImageHud(const std::shared_ptr<ImageHud>& image) =0;

        virtual void unmanageCircle(const std::shared_ptr<Circle>& circle) =0;
        virtual void unmanagePolygon(const std::shared_ptr<Polygon>& polygon) =0;
        virtual void unmanageTextHud(const std::shared_ptr<TextHud>& text) =0;
        virtual void unmanageImageHud(const std::shared_ptr<ImageHud>& image) =0;
    };
}

#endif // PROPROOM2D_ABSTRACTARTDIRECTOR_H
