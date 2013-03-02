#ifndef CELLARWORKBENCH_CAMERAMANBIRD_H
#define CELLARWORKBENCH_CAMERAMANBIRD_H

#include "libCellarWorkbench_global.h"
#include "DataStructure/Vector.h"


namespace cellar
{
    class Camera;


    class CELLAR_EXPORT CameraManBird
    {
    public:
        CameraManBird();
        CameraManBird(Camera& camera);

        void setCamera(Camera& camera);

        void moveTo(const Vec3f& destination);
        void moveBy(const Vec2f& displacement);
        void rotate(float radians);

    private:
        void setupCamera();

        Camera* _camera;
    };
}

#endif // CELLARWORKBENCH_CAMERAMANBIRD_H
