#include "CameraManFree.h"

#include <cmath>

#include <GLM/gtc/matrix_transform.hpp>

#include "Camera.h"


namespace cellar
{
    CameraManFree::CameraManFree(const std::shared_ptr<Camera>& camera) :
        _camera(camera),
        _tiltAngle(0),
        _panAngle(0),
        _position(),
        _fieldOfView(0.78f),
        _nearPlane(0.1f),
        _farPlane(300.0f)
    {
        _camera->registerObserver(*this);
        updateProjection();
    }

    void CameraManFree::setFieldOfView(float radians)
    {
        _fieldOfView = radians;
        updateProjection();
    }

    void CameraManFree::setCliPlanes(float nearPlane, float farPlane)
    {
        _nearPlane = nearPlane;
        _farPlane = farPlane;
        updateProjection();
    }

    void CameraManFree::setProjection(float fovRad, float nearPlane, float farPlane)
    {
        _fieldOfView = fovRad;
        _nearPlane = nearPlane;
        _farPlane = farPlane;
        updateProjection();
    }

    void CameraManFree::forward(float dist)
    {
        _position += glm::vec3(_orientation * glm::vec4(0, dist, 0, 0));
        updateView();
    }

    void CameraManFree::sideward(float dist)
    {
        _position += glm::vec3(_orientation * glm::vec4(dist, 0, 0, 0));
        updateView();
    }

    void CameraManFree::pan(float radians)
    {
        _panAngle += radians;
        updateOrientation();
        updateView();
    }

    void CameraManFree::tilt(float radians)
    {
        _tiltAngle += radians;
        updateOrientation();
        updateView();
    }

    void CameraManFree::setPosition(const glm::vec3& position)
    {
        _position = position;
        updateView();
    }

    void CameraManFree::setOrientation(float panRadians, double tiltRadians)
    {
        _tiltAngle = tiltRadians;
        _panAngle = panRadians;
        updateOrientation();
        updateView();
    }

    void CameraManFree::updateView()
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

    void CameraManFree::updateProjection()
    {
        if(_camera == nullptr)
            return;

        float width = _camera->viewport().x;
        float height = _camera->viewport().y;
        _camera->updateProjection(
                    glm::perspectiveFov(
                        _fieldOfView,
                        width,
                        height,
                        _nearPlane,
                        _farPlane));
    }

    void CameraManFree::notify(CameraMsg& msg)
    {
        if(msg.change == CameraMsg::EChange::VIEWPORT)
        {
            updateProjection();
        }
    }
}
