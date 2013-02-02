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

    void CameraManBird::moveTo(const Vector3D<float>& destination)
    {
        if(_camera == nullptr)
            return;

        Vector3D<float> displacement = destination - _camera->tripod().from();
        _camera->setTripod(destination,
                           _camera->tripod().to() + displacement,
                           _camera->tripod().up());
    }

    void CameraManBird::moveBy(const Vector2D<float>& displacement)
    {
        if(_camera == nullptr)
            return;

        Vector3D<float> front = _camera->tripod().to() - _camera->tripod().from();
        Vector3D<float> up = _camera->tripod().up();
        Vector3D<float> side = front ^ up;

        Vector3D<float> move = displacement.x()*side + displacement.y()*up;

        _camera->setTripod(_camera->tripod().from() + move,
                           _camera->tripod().to() + move,
                           _camera->tripod().up());
    }

    void CameraManBird::rotate(float radians)
    {
        if(_camera == nullptr)
            return;

        Vector3D<float> front = (_camera->tripod().to() - _camera->tripod().from()).normalized();
        _camera->setTripod(_camera->tripod().from(),
                           _camera->tripod().to(),
                           Vector3D<float>(_camera->tripod().up()).rotate(front, radians));
    }

    void CameraManBird::setupCamera()
    {
        if(_camera == nullptr)
            return;

        _camera->setMode(Camera::FRAME);

        _camera->setFrame(_camera->knowWindowWidth(),
                          _camera->knowWindowHeight());

        _camera->setLens(Camera::Lens::ORTHOGRAPHIC,
                         -_camera->knowWindowWidth() / 2.0, _camera->knowWindowWidth() / 2.0,
                         -_camera->knowWindowHeight() / 2.0, _camera->knowWindowHeight() / 2.0,
                         -1.0, 1.0);

        _camera->setTripod(Vector3D<float>(_camera->knowWindowWidth() / 2.0f,
                                           _camera->knowWindowHeight() / 2.0f,
                                           0.0f),
                           Vector3D<float>(_camera->knowWindowWidth() / 2.0f,
                                           _camera->knowWindowHeight() / 2.0f,
                                           -1.0f),
                           Vector3D<float>(0.0f, 1.0f, 0.0f));
    }
}
