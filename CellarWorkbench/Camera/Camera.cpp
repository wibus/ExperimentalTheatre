#include "Camera.h"


namespace cellar
{
    Camera::Camera(
        const glm::ivec2& viewport,
        const glm::mat4& projection,
        const glm::mat4& view) :
        _viewport(viewport),
        _projMatrix(projection),
        _viewMatrix(view)
    {
    }

    void Camera::refresh()
    {
        CameraMsg msg1(*this, CameraMsg::EChange::VIEWPORT);
        notifyObservers(msg1);
        CameraMsg msg2(*this, CameraMsg::EChange::PROJECTION);
        notifyObservers(msg2);
        CameraMsg msg3(*this, CameraMsg::EChange::VIEW);
        notifyObservers(msg3);
    }

    void Camera::updateViewport(int width, int height)
    {
        _viewport = glm::ivec2(width, height);
        CameraMsg msg(*this, CameraMsg::EChange::VIEWPORT);
        notifyObservers(msg);
    }

    void Camera::updateViewport(const glm::ivec2& viewport)
    {
        _viewport = viewport;
        CameraMsg msg(*this, CameraMsg::EChange::VIEWPORT);
        notifyObservers(msg);
    }

    void Camera::updateProjection(const glm::mat4& proj)
    {
        _projMatrix = proj;
        CameraMsg msg(*this, CameraMsg::EChange::PROJECTION);
        notifyObservers(msg);
    }

    void Camera::updateView(const glm::mat4& view)
    {
        _viewMatrix = view;
        CameraMsg msg(*this, CameraMsg::EChange::VIEW);
        notifyObservers(msg);
    }
}
