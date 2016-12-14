#ifndef CELLARWORKBENCH_CAMERAMANBIRD_H
#define CELLARWORKBENCH_CAMERAMANBIRD_H

#include <memory>

#include <GLM/glm.hpp>

#include "../libCellarWorkbench_global.h"
#include "../DesignPattern/SpecificObserver.h"


namespace cellar
{
    class Camera;
    class CameraMsg;


    class CELLAR_EXPORT CameraManBird : private SpecificObserver<CameraMsg>
    {
    public:
        CameraManBird(const std::shared_ptr<Camera>& camera);
        virtual ~CameraManBird();

        void setZoom(const glm::vec2& unitsPerPixel);
        void moveTo(const glm::vec2& destination);
        void moveBy(const glm::vec2& displacement);
        void rotate(float radians);

    private:
        virtual void notify(CameraMsg& msg) override;
        void updateProjection();

        std::shared_ptr<Camera> _camera;
        glm::vec2 _unitsPerPixelZoom;
    };
}

#endif // CELLARWORKBENCH_CAMERAMANBIRD_H
