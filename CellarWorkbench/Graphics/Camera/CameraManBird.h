#ifndef CELLARWORKBENCH_CAMERAMANBIRD_H
#define CELLARWORKBENCH_CAMERAMANBIRD_H

#include "libCellarWorkbench_global.h"
#include "DataStructure/Vector2D.h"
#include "DataStructure/Vector3D.h"


namespace cellar
{
    class Camera;


    class CELLAR_EXPORT CameraManBird
    {
    public:
        CameraManBird();
        CameraManBird(Camera& camera);

        void setCamera(Camera& camera);

        void moveTo(const Vector3D<float>& destination);
        void moveBy(const Vector2D<float>& displacement);
        void rotate(float radians);

    private:
        void setupCamera();

        Camera* _camera;
    };
}

#endif // CELLARWORKBENCH_CAMERAMANBIRD_H
