#ifndef CELLARWORKBENCH_CAMERAMANFREE_H
#define CELLARWORKBENCH_CAMERAMANFREE_H

#include "libCellarWorkbench_global.h"


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
        void turnHorizontaly(float radians);
        void turnVerticaly(float radians);

    private:
        void setupCamera();

        Camera* _camera;
    };
}

#endif // CELLARWORKBENCH_CAMERAMANFREE_H
