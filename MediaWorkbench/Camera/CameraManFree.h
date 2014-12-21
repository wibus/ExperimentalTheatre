#ifndef MEDIAWORKBENCH_CAMERAMANFREE_H
#define MEDIAWORKBENCH_CAMERAMANFREE_H

#include "../libMediaWorkbench_global.h"


namespace media
{
    class Camera;


    class MEDIA_EXPORT CameraManFree
    {
    public:
        CameraManFree();
        CameraManFree(Camera& camera);

        void setCamera(Camera& camera);
        void setFov(float radians, float nearPlane, float farPlane);

        void forward(float dist);
        void sideward(float dist);
        void turnHorizontaly(float radians);
        void turnVerticaly(float radians);

    private:
        void setupCamera();

        Camera* _camera;
    };
}

#endif // MEDIAWORKBENCH_CAMERAMANFREE_H
