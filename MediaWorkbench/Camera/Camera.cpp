#include "Camera.h"


namespace media
{
    // CAMERA //
    Camera::Camera() :
        _mode(EMode::EXPAND),
        _frame(),
        _lens(),
        _tripod(),
        _viewport(1, 1),
        _projMatrix(),
        _viewMatrix()
    {
        refresh();
    }

    Camera::Camera(EMode mode, const Frame& frame,
                   const Lens &lens, const Tripod &tripod,
                   const cellar::Vec2i& viewport) :
        _mode(mode),
        _frame(frame),
        _lens(lens),
        _tripod(tripod),
        _viewport(viewport),
        _projMatrix(),
        _viewMatrix()
    {
        refresh();
    }


    void Camera::setMode(EMode mode)
    {
        if(_mode != mode)
        {
            _mode = mode;
            refreshProjection();
        }
    }

    void Camera::setFrame(const cellar::Vec2f& size)
    {
        setFrame(size, _frame.center());
    }

    void Camera::setFrame(const cellar::Vec2f& size,
                          const cellar::Vec2f& center)
    {
        if(size   != _frame.size() ||
           center != _frame.center())
        {
            _frame.set(size, center);
            if(_mode == EMode::FRAME)
            {
                refreshProjection();
            }
        }
    }

    void Camera::setLens(Lens::EType type)
    {
        if(type != _lens.type())
        {
            setLens(type,
                    _lens.left(),      _lens.right(),
                    _lens.bottom(),    _lens.top(),
                    _lens.nearPlane(), _lens.farPlane());
        }
    }

    void Camera::setLens(float nearPlane,
                         float farPlane)
    {
        if(nearPlane != _lens.nearPlane() ||
           farPlane  != _lens.farPlane())
        {
            setLens(_lens.type(),
                    _lens.left(),   _lens.right(),
                    _lens.bottom(), _lens.top(),
                    nearPlane,      farPlane);
        }
    }

    void Camera::setLens(float left,
                         float right,
                         float bottom,
                         float top)
    {
        setLens(_lens.type(),
                left,              right,
                bottom,            top,
                _lens.nearPlane(), _lens.farPlane());
    }

    void Camera::setLens(Lens::EType type,
                         float left,
                         float right,
                         float bottom,
                         float top,
                         float nearPlane,
                         float farPlane)
    {
        _lens.set(type,
                  left,      right,
                  bottom,    top,
                  nearPlane, farPlane);
        refreshProjection();
    }

    void Camera::setTripod(const cellar::Vec3f& from,
                           const cellar::Vec3f& to,
                           const cellar::Vec3f& up)
    {
        _tripod.set(from, to, up);
        updateViewMatrix();
    }

    void Camera::moveBy(const cellar::Vec3f& dist)
    {
        _tripod.moveBy( dist );
        updateViewMatrix();
    }

    void Camera::refresh()
    {
        refreshProjection();
        updateViewMatrix();
    }

    void Camera::refreshProjection()
    {
        updateViewport(_viewport.x(), _viewport.y());
    }

    void Camera::updateViewport(int width, int height)
    {
        if(width == 0 || height == 0)
            return;

        cellar::Vec2i oldViewport(_viewport);
        _viewport(width, height);

        if(_mode == EMode::EXPAND)
        {
            float wRatio = static_cast<float>(width) / oldViewport.x();
            float hRatio = static_cast<float>(height) / oldViewport.y();

            _lens.set(_lens.type(),
                      _lens.left()   * wRatio, _lens.right() * wRatio,
                      _lens.bottom() * hRatio, _lens.top()   * hRatio,
                      _lens.nearPlane(),       _lens.farPlane());
        }
        else if(_mode == EMode::FRAME)
        {
            float dW, dH;
            cellar::Vec2f frameCenter = _frame.center();
            float viewportAspectRatio = static_cast<float>(width) / height;
            if(viewportAspectRatio < _frame.aspectRatio())
            {
                dW = _frame.size().x();
                dH = dW / viewportAspectRatio;
                frameCenter.setY(frameCenter.y() / viewportAspectRatio);
            }
            else
            {
                dH = _frame.size().y();
                dW = dH * viewportAspectRatio;
                frameCenter.setX(frameCenter.x() * viewportAspectRatio);
            }

            _lens.set(_lens.type(),
                      -frameCenter.x(), dW - frameCenter.x(),
                      -frameCenter.y(), dH - frameCenter.y(),
                      _lens.nearPlane(),    _lens.farPlane());
        }
        else if(_mode == EMode::STRETCH)
        {
            // Let stretch
        }

        updateProjectionMatrix();
    }

    void Camera::updateMatrices()
    {
        updateProjectionMatrix();
        updateViewMatrix();
    }

    void Camera::updateProjectionMatrix()
    {
        if(_lens.type() == Lens::EType::ORTHOGRAPHIC)
            _projMatrix = cellar::ortho(
                _lens.left(),      _lens.right(),
                _lens.bottom(),    _lens.top(),
                _lens.nearPlane(), _lens.farPlane());

        else if(_lens.type() == Lens::EType::PERSPECTIVE)
            _projMatrix = cellar::frustrum(
                _lens.left(),      _lens.right(),
                _lens.bottom(),    _lens.top(),
                _lens.nearPlane(), _lens.farPlane());

        setIsChanged( true );
        CameraMsg msg(*this, CameraMsg::EChange::PROJECTION);
        notifyObservers( msg );
    }

    void Camera::updateViewMatrix()
    {
        _viewMatrix = lookAt(
            _tripod.from(),
            _tripod.to(),
            _tripod.up());

        setIsChanged( true );
        CameraMsg msg(*this, CameraMsg::EChange::VIEW);
        notifyObservers( msg);
    }


    // CAMERA::FRAME //
    Camera::Frame::Frame(const cellar::Vec2f& size,
                         const cellar::Vec2f& center) :
        _size(size),
        _center(center)
    {
    }

    // CAMERA::LENS //
    Camera::Lens::Lens() :
        _type(EType::ORTHOGRAPHIC),
        _left(0.0),
        _right(1.0),
        _bottom(0.0),
        _top(1.0),
        _nearPlane(-1.0),
        _farPlane(1.0)
    {
    }

    Camera::Lens::Lens(EType type,
         float left,       float right,
         float bottom,     float top,
         float nearPlane,  float farPlane) :
        _type(type),
        _left(left),
        _right(right),
        _bottom(bottom),
        _top(top),
        _nearPlane(nearPlane),
        _farPlane(farPlane)
    {
    }

    void Camera::Lens::set(EType type,
                           float left,      float right,
                           float bottom,    float top,
                           float nearPlane, float farPlane)
    {
        _type = type;
        _left = left;           _right = right;
        _bottom = bottom;       _top = top;
        _nearPlane = nearPlane; _farPlane = farPlane;
    }


    // CAMERA::TRIPOD //
    Camera::Tripod::Tripod() :
        _from(0.0, 0.0, 0.0),
        _to(0.0, 0.0, -1.0),
        _up(0.0, 1.0, 0.0)
    {
    }

    Camera::Tripod::Tripod(const cellar::Vec3f& from,
                           const cellar::Vec3f& to,
                           const cellar::Vec3f& up) :
        _from(from),
        _to(to),
        _up(up)
    {}
}
