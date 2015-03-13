#ifndef MEDIAWORKBENCH_CAMERA_H
#define MEDIAWORKBENCH_CAMERA_H

#include <GLM/glm.hpp>

#include <CellarWorkbench/DesignPattern/SpecificSubject.h>
#include "../libMediaWorkbench_global.h"


namespace media
{
    struct CameraMsg;

    class MEDIA_EXPORT Camera : public cellar::SpecificSubject<CameraMsg>
    {
    public:
        explicit Camera(
            const glm::ivec2& viewport = glm::ivec2(1, 1),
            const glm::mat4& projection = glm::mat4(),
            const glm::mat4& view = glm::mat4());

        // Notifications
        void refresh();

        void updateViewport(int width, int height);
        void updateViewport(const glm::ivec2& viewport);
        void updateProjection(const glm::mat4& proj);
        void updateView(const glm::mat4& view);

        const glm::ivec2& viewport() const;
        const glm::mat4& projectionMatrix() const;
        const glm::mat4& viewMatrix() const;


    private:
        glm::ivec2 _viewport;
        glm::mat4 _projMatrix;
        glm::mat4 _viewMatrix;
    };

    struct CameraMsg
    {
        enum class EChange {VIEWPORT, PROJECTION, VIEW};

        Camera& camera;
        EChange change;

    private:
        friend class Camera;
        CameraMsg(Camera& camera, EChange change) :
            camera(camera), change(change) {}
    };



    // IMPLEMENTATION //
    inline const glm::ivec2& Camera::viewport() const
    {
        return _viewport;
    }

    inline const glm::mat4& Camera::projectionMatrix() const
    {
        return _projMatrix;
    }

    inline const glm::mat4& Camera::viewMatrix() const
    {
        return _viewMatrix;
    }
}


#endif // MEDIAWORKBENCH_CAMERA_H
