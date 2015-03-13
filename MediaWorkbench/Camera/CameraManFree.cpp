#include "CameraManFree.h"

#include <cmath>

#include <GLM/gtc/matrix_transform.hpp>

#include "Camera.h"


namespace media
{
    CameraManFree::CameraManFree() :
        _camera(nullptr)
    {
    }

    CameraManFree::CameraManFree(Camera& camera) :
        _camera(&camera)
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
        glm::mat4 tran = glm::translate(glm::mat4(), glm::vec3(0, 0, dist));
        _camera->updateView(tran * _camera->viewMatrix());
    }

    void CameraManFree::sideward(float dist)
    {
        glm::mat4 tran = glm::translate(glm::mat4(), glm::vec3(-dist, 0, 0));
        _camera->updateView(tran * _camera->viewMatrix());
    }

    void CameraManFree::pan(float radians)
    {
        glm::mat4 rot = glm::rotate(glm::mat4(), radians, glm::vec3(0, 1, 0));
        _camera->updateView(rot * _camera->viewMatrix());
    }

    void CameraManFree::tilt(float radians)
    {
        glm::mat4 rot = glm::rotate(glm::mat4(), radians, glm::vec3(1, 0, 0));
        _camera->updateView(rot * _camera->viewMatrix());
    }
}
