#ifndef CELLARWORKBENCH_CAMERAMANFREE_H
#define CELLARWORKBENCH_CAMERAMANFREE_H

#include <GLM/glm.hpp>


#include "../libCellarWorkbench_global.h"


namespace cellar
{
    class Camera;


    class CELLAR_EXPORT CameraManFree
    {
    public:
        CameraManFree();
        CameraManFree(Camera& camera);

        void setCamera(Camera& camera);
        void setFov(float radians, float nearPlane, float farPlane);

        void forward(float dist);
        void sideward(float dist);
        void pan(float radians);
        void tilt(float radians);

        void setPosition(const glm::vec3& position);
        void setOrientation(float panRadians, double tiltRadians);

    private:
        void updateCamera();
        void updateOrientation();

        Camera* _camera;
        float _panAngle;
        float _tiltAngle;
        glm::vec3 _position;
        glm::mat4 _orientation;
    };
}

#endif // CELLARWORKBENCH_CAMERAMANFREE_H
