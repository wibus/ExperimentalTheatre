#include "CameraManFree.h"

#include <cmath>

#include <GLM/gtc/matrix_transform.hpp>

#include "Camera.h"


namespace cellar
{
    CameraManFree::CameraManFree() :
        _camera(nullptr),
        _tiltAngle(0),
        _panAngle(0),
        _position()
    {
    }

    CameraManFree::CameraManFree(Camera& camera) :
        _camera(&camera),
        _tiltAngle(0),
        _panAngle(0),
        _position()
    {
        setFov(0.78f, 0.3f, 300.0f);
    }

    void CameraManFree::setCamera(Camera& camera)
    {
        _camera = &camera;
        setFov(0.78f, 0.3f, 300.0f);
    }

    void CameraManFree::setFov(float radians, float nearPlane, float farPlane)
    {
        if(_camera == nullptr)
            return;

        float w = _camera->viewport().x;
        float h = _camera->viewport().y;

        _camera->updateProjection(
            glm::perspectiveFov(radians, w, h, nearPlane, farPlane));
    }

    void CameraManFree::forward(float dist)
    {
        _position += glm::vec3(_orientation * glm::vec4(0, dist, 0, 0));
        updateCamera();
    }

    void CameraManFree::sideward(float dist)
    {
        _position += glm::vec3(_orientation * glm::vec4(dist, 0, 0, 0));
        updateCamera();
    }

    void CameraManFree::pan(float radians)
    {
        _panAngle += radians;
        updateOrientation();
        updateCamera();
    }

    void CameraManFree::tilt(float radians)
    {
        _tiltAngle += radians;
        updateOrientation();
        updateCamera();
    }

    void CameraManFree::setPosition(const glm::vec3& position)
    {
        _position = position;
        updateCamera();
    }

    void CameraManFree::setOrientation(float panRadians, double tiltRadians)
    {
        _tiltAngle = tiltRadians;
        _panAngle = panRadians;
        updateOrientation();
        updateCamera();
    }

    void CameraManFree::updateCamera()
    {
        glm::mat4 view;
        view = glm::rotate(view, -glm::pi<float>()/2.0f - _tiltAngle, glm::vec3(1, 0, 0));
        view = glm::rotate(view, -_panAngle, glm::vec3(0, 0, 1));
        _camera->updateView(glm::translate(view, -_position));
    }

    void CameraManFree::updateOrientation()
    {
        _orientation = glm::mat4();
        _orientation = glm::rotate(_orientation, _panAngle,  glm::vec3(0, 0, 1));
        _orientation = glm::rotate(_orientation, _tiltAngle, glm::vec3(1, 0, 0));
    }
}
