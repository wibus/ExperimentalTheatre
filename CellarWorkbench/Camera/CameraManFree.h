#ifndef CELLARWORKBENCH_CAMERAMANFREE_H
#define CELLARWORKBENCH_CAMERAMANFREE_H

#include <memory>

#include <GLM/glm.hpp>

#include "../libCellarWorkbench_global.h"
#include "../DesignPattern/SpecificObserver.h"


namespace cellar
{
    class Camera;
    class CameraMsg;


    class CELLAR_EXPORT CameraManFree : private SpecificObserver<CameraMsg>
    {
    public:
        CameraManFree(const std::shared_ptr<Camera>& camera, bool manageProjection = true);

        void setFieldOfView(float radians);
        void setCliPlanes(float nearPlane, float farPlane);
        void setProjection(float fovRad, float nearPlane, float farPlane);

        void forward(float dist);
        void sideward(float dist);
        void pan(float radians);
        void tilt(float radians);

        glm::vec3 position() const;
        void setPosition(const glm::vec3& position);
        float panOrientation() const;
        float tiltOrientation() const;
        void setOrientation(float panRadians, double tiltRadians);

    private:
        virtual void notify(CameraMsg& msg);

        void updateView();
        void updateOrientation();
        void updateProjection();

        std::shared_ptr<Camera> _camera;
        bool _manageProjection;
        float _panAngle;
        float _tiltAngle;
        glm::vec3 _position;
        glm::mat4 _orientation;
        float _fieldOfView;
        float _nearPlane;
        float _farPlane;
    };



    // IMPLEMENTATION //
    inline glm::vec3 CameraManFree::position() const
    {
        return _position;
    }

    inline float CameraManFree::panOrientation() const
    {
        return _panAngle;
    }

    inline float CameraManFree::tiltOrientation() const
    {
        return _tiltAngle;
    }
}

#endif // CELLARWORKBENCH_CAMERAMANFREE_H
