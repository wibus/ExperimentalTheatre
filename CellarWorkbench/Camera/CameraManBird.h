#ifndef CELLARWORKBENCH_CAMERAMANBIRD_H
#define CELLARWORKBENCH_CAMERAMANBIRD_H

#include <GLM/glm.hpp>


#include "../libCellarWorkbench_global.h"


namespace cellar
{
    class Camera;


    class CELLAR_EXPORT CameraManBird
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

#endif // CELLARWORKBENCH_CAMERAMANBIRD_H
