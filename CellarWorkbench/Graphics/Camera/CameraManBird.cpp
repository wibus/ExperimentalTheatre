#include "CameraManBird.h"

#include "Camera.h"


namespace cellar
{
    CameraManBird::CameraManBird() :
        _camera(nullptr)
    {
    }

    CameraManBird::CameraManBird(Camera& camera) :
        _camera(nullptr)
    {
        setCamera( camera );
    }

    void CameraManBird::setCamera(Camera& camera)
    {
        _camera = & camera;
        setupCamera();
    }

    void CameraManBird::moveTo(const Vec3f& destination)
    {
        if(_camera == nullptr)
            return;

        Vec3f displacement = destination - _camera->tripod().from();
        _camera->setTripod(destination,
                           _camera->tripod().to() + displacement,
                           _camera->tripod().up());
    }

    void CameraManBird::moveBy(const Vec2f& displacement)
    {
        if(_camera == nullptr)
            return;

        Vec3f front = _camera->tripod().to() - _camera->tripod().from();
        Vec3f up = _camera->tripod().up();
        Vec3f side = cross(front, up);

        Vec3f move = displacement.x()*side + displacement.y()*up;

        _camera->setTripod(_camera->tripod().from() + move,
                           _camera->tripod().to() + move,
                           _camera->tripod().up());
    }

    void CameraManBird::rotate(float radians)
    {
        if(_camera == nullptr)
            return;

        Vec3f front = (_camera->tripod().to() - _camera->tripod().from()).normalized();
        Vec3f nUp = cellar::rotate(front.x(), front.y(), front.z(), radians) * _camera->tripod().up();
        _camera->setTripod(_camera->tripod().from(),
                           _camera->tripod().to(),
                           nUp);
    }

    void CameraManBird::setupCamera()
    {
        if(_camera == nullptr)
            return;

        _camera->setMode(Camera::FRAME);

        _camera->setFrame((float)_camera->knowWindowWidth(),
                          (float)_camera->knowWindowHeight());

        _camera->setLens(Camera::Lens::ORTHOGRAPHIC,
                         -_camera->knowWindowWidth() / 2.0f,  _camera->knowWindowWidth() / 2.0f,
                         -_camera->knowWindowHeight() / 2.0f, _camera->knowWindowHeight() / 2.0f,
                         -1.0f, 1.0f);

        _camera->setTripod(Vec3f(_camera->knowWindowWidth() / 2.0f,
                                           _camera->knowWindowHeight() / 2.0f,
                                           0.0f),
                           Vec3f(_camera->knowWindowWidth() / 2.0f,
                                           _camera->knowWindowHeight() / 2.0f,
                                           -1.0f),
                           Vec3f(0.0f, 1.0f, 0.0f));
    }
}
