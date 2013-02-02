#include "CameraManFree.h"
#include "Camera.h"

#include <cmath>


namespace cellar
{
    CameraManFree::CameraManFree() :
        _camera(nullptr)
    {
    }

    CameraManFree::CameraManFree(Camera& camera) :
        _camera(nullptr)
    {
        setCamera( camera );
    }

    void CameraManFree::setCamera(Camera& camera)
    {
        _camera = &camera;
        setupCamera();
    }

    void CameraManFree::setFov(float radians, float nearPlane, float farPlane)
    {
        if(_camera == nullptr)
            return;

        float aspectRatio = _camera->lens().width() / _camera->lens().height();
        float halfScreen = tan(radians / 2.0f) * nearPlane;

        _camera->setLens(Camera::Lens::PERSPECTIVE,
                         -halfScreen,                halfScreen,
                         -halfScreen / aspectRatio, halfScreen / aspectRatio,
                         nearPlane,                 farPlane);

        _camera->setTripod(Vector3D<float>(0, 0, 0),
                           Vector3D<float>(0, 1, 0),
                           Vector3D<float>(0, 0, 1));
    }

    void CameraManFree::forward(float dist)
    {
        Vector3D<float> dir =
            (_camera->tripod().to() - _camera->tripod().from()).normalized();

        _camera->moveBy( dir * dist );
    }

    void CameraManFree::sideward(float dist)
    {
        Vector3D<float> dir =
            (_camera->tripod().to() - _camera->tripod().from()).normalized();
        Vector3D<float> side = (dir ^ _camera->tripod().up()).normalized();

        _camera->moveBy( side * dist );
    }

    void CameraManFree::turnHorizontaly(float radians)
    {
        Vector3D<float> dir =
            (_camera->tripod().to() - _camera->tripod().from()).normalized();
        Vector3D<float> side = -(dir ^ _camera->tripod().up()).normalized();

        Vector3D<float> newDir =  cos(radians) * dir +  sin(radians) * side;

        _camera->setTripod(_camera->tripod().from(),
                           _camera->tripod().from() + newDir,
                           _camera->tripod().up());
    }

    void CameraManFree::turnVerticaly(float radians)
    {
        Vector3D<float> dir =
            (_camera->tripod().to() - _camera->tripod().from()).normalized();
        Vector3D<float> newDir =  dir * cos(radians) +
                                  _camera->tripod().up() * sin(radians);

        _camera->setTripod(_camera->tripod().from(),
                           _camera->tripod().from() + newDir,
                           _camera->tripod().up());
    }

    void CameraManFree::setupCamera()
    {
        _camera->setMode(Camera::EXPAND);
        setFov(0.78f, 0.3f, 300.0f);
    }
}
