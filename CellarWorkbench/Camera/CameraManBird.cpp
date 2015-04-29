#include "CameraManBird.h"

#include <GLM/gtc/matrix_transform.hpp>

#include "Camera.h"


namespace cellar
{
    CameraManBird::CameraManBird(Camera& camera) :
        _camera(&camera)
    {
        float w = _camera->viewport().x;
        float h = _camera->viewport().y;

        _camera->updateProjection(
            glm::ortho(-w/2, -h/2,
                        w/2,  h/2,
                       -1.0f, 1.0f));

        _camera->updateView(
            glm::lookAt(glm::vec3(w/2.0f, h/2.0f, 1),
                        glm::vec3(w/2.0f, h/2.0f, 0),
                        glm::vec3(0,      1,      0)));
    }

    void CameraManBird::moveTo(const glm::vec3& destination)
    {
        float x = destination.x;
        float y = destination.y;

        _camera->updateView(
            glm::lookAt(glm::vec3(x, y, 1),
                        glm::vec3(x, y, 0),
                        glm::vec3(0, 1, 0)));
    }

    void CameraManBird::moveBy(const glm::vec2& displacement)
    {
        glm::mat4 tran = glm::translate(glm::mat4(), glm::vec3(displacement, 0));
        _camera->updateView(tran * _camera->viewMatrix());
    }

    void CameraManBird::rotate(float radians)
    {
       glm::mat4 rot = glm::rotate(glm::mat4(), radians, glm::vec3(0, 0, 1));
       _camera->updateView(rot * _camera->viewMatrix());
    }
}
