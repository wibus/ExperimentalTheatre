#ifndef MEDIAWORKBENCH_CAMERAMANBIRD_H
#define MEDIAWORKBENCH_CAMERAMANBIRD_H

#include <GLM/glm.hpp>

#include "../libMediaWorkbench_global.h"


namespace media
{
    class Camera;


    class MEDIA_EXPORT CameraManBird
    {
    public:
        CameraManBird(Camera& camera);

        void moveTo(const glm::vec3& destination);
        void moveBy(const glm::vec2& displacement);
        void rotate(float radians);

    private:
        Camera* _camera;
    };
}

#endif // MEDIAWORKBENCH_CAMERAMANBIRD_H
