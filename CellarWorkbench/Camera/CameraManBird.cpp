#include "CameraManBird.h"

#include <GLM/gtc/matrix_transform.hpp>

#include "Camera.h"


namespace cellar
{
    CameraManBird::CameraManBird(const std::shared_ptr<Camera>& camera) :
        _camera(camera),
        _unitsPerPixelZoom(1.0f, 1.0f)
    {
        _camera->registerObserver(*this);
        moveTo(glm::vec2(_camera->viewport()) / 2.0f);
        updateProjection();
    }

    CameraManBird::~CameraManBird()
    {
        _camera->unregisterObserver(*this);
    }

    void CameraManBird::setZoom(const glm::vec2& unitsPerPixel)
    {
        _unitsPerPixelZoom = unitsPerPixel;
        updateProjection();
    }

    void CameraManBird::moveTo(const glm::vec2& destination)
    {
        float x = destination.x;
        float y = destination.y;

        _camera->updateView(
            glm::lookAt(glm::vec3(x, y, 0),
                        glm::vec3(x, y, -1),
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

    void CameraManBird::notify(CameraMsg& msg)
    {
        if(msg.change == CameraMsg::EChange::VIEWPORT)
        {
            updateProjection();
        }
    }

    void CameraManBird::updateProjection()
    {
         const glm::ivec2& v = _camera->viewport();
        glm::vec2 size =  glm::vec2(v) * _unitsPerPixelZoom;

        _camera->updateProjection(
            glm::ortho(-size.x/2, size.x/2,
                       -size.y/2, size.y/2,
                       -1.0f,      1.0f));
    }
}
