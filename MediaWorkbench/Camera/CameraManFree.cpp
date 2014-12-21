#include "CameraManFree.h"

#include <cmath>

#include "Camera.h"


namespace media
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

        _camera->setLens(Camera::Lens::EType::PERSPECTIVE,
                         -halfScreen,               halfScreen,
                         -halfScreen / aspectRatio, halfScreen / aspectRatio,
                         nearPlane,                 farPlane);

        _camera->setTripod(cellar::Vec3f(0, 0, 0),
                           cellar::Vec3f(0, 1, 0),
                           cellar::Vec3f(0, 0, 1));
    }

    void CameraManFree::forward(float dist)
    {
        cellar::Vec3f dir =
            (_camera->tripod().to() - _camera->tripod().from()).normalized();

        _camera->moveBy( dir * dist );
    }

    void CameraManFree::sideward(float dist)
    {
        cellar::Vec3f dir =
            (_camera->tripod().to() - _camera->tripod().from()).normalized();
        cellar::Vec3f side = cross(dir, _camera->tripod().up()).normalized();

        _camera->moveBy( side * dist );
    }

    void CameraManFree::turnHorizontaly(float radians)
    {
        cellar::Vec3f dir =
            (_camera->tripod().to() - _camera->tripod().from()).normalized();
        cellar::Vec3f side = -cross(dir, _camera->tripod().up()).normalized();

        cellar::Vec3f newDir =  dir  *  (float)cos(radians) +
                                side * (float)sin(radians);

        _camera->setTripod(_camera->tripod().from(),
                           _camera->tripod().from() + newDir,
                           _camera->tripod().up());
    }

    void CameraManFree::turnVerticaly(float radians)
    {
        cellar::Vec3f dir =
            (_camera->tripod().to() - _camera->tripod().from()).normalized();
        cellar::Vec3f newDir =  dir *                    (float)cos(radians) +
                                _camera->tripod().up() * (float)sin(radians);

        _camera->setTripod(_camera->tripod().from(),
                           _camera->tripod().from() + newDir,
                           _camera->tripod().up());
    }

    void CameraManFree::setupCamera()
    {
        _camera->setMode(Camera::EMode::EXPAND);
        setFov(0.78f, 0.3f, 300.0f);
    }
}
