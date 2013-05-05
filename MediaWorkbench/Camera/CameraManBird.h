#ifndef MEDIAWORKBENCH_CAMERAMANBIRD_H
#define MEDIAWORKBENCH_CAMERAMANBIRD_H

#include "libMediaWorkbench_global.h"
#include "DataStructure/Vector.h"


namespace media
{
    class Camera;


    class MEDIA_EXPORT CameraManBird
    {
    public:
        CameraManBird();
        CameraManBird(Camera& camera);

        void setCamera(Camera& camera);

        void moveTo(const cellar::Vec3f& destination);
        void moveBy(const cellar::Vec2f& displacement);
        void rotate(float radians);

    private:
        void setupCamera();

        Camera* _camera;
    };
}

#endif // MEDIAWORKBENCH_CAMERAMANBIRD_H
