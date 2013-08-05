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
        updateMatrices();
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
        updateMatrices();
    }


    void Camera::setMode(EMode mode)
    {
        if(_mode != mode)
        {
            _mode = mode;
            updateViewport(_viewport.x(), _viewport.y());
        }
    }

    void Camera::setFrame(float width, float height)
    {
        if(_frame.width() != width && _frame.height() != height)
        {
            _frame.set(width, height);
            if(_mode == EMode::FRAME)
            {
                updateViewport(_viewport.x(), _viewport.y());
            }
        }
    }

    void Camera::setLens(float left,         float right,
                         float bottom,       float top)
    {
        setLens(_lens.type(),
                left,              right,
                bottom,            top,
                _lens.nearPlane(), _lens.farPlane());
    }

    void Camera::setLens(Lens::EType type,
                         float left,         float right,
                         float bottom,       float top,
                         float nearPlane,    float farPlane)
    {
        _lens.set(type, left, right, bottom, top, nearPlane, farPlane);
        updateProjectionMatrix();
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
        updateViewport(_viewport.x(), _viewport.y());
        updateViewMatrix();
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

            setLens(_lens.left() * wRatio,   _lens.right() * wRatio,
                    _lens.bottom() * hRatio, _lens.top() * hRatio);
        }
        else if(_mode == EMode::FRAME)
        {
            float aspectRatio = static_cast<float>(width) / height;
            float dW, dH;

            if( aspectRatio < (_frame.width() / _frame.height()) )
            {
                dW = _frame.width();
                dH = dW / aspectRatio;
            }
            else
            {
                dH = _frame.height();
                dW = dH * aspectRatio;
            }

            float wRatio = static_cast<float>(dW) / (_lens.right() - _lens.left());
            float hRatio = static_cast<float>(dH) / (_lens.top() - _lens.bottom());

            setLens(_lens.left() * wRatio,   _lens.right() * wRatio,
                    _lens.bottom() * hRatio, _lens.top() * hRatio);
        }
        else if(_mode == EMode::STRETCH)
        {
            // Let stretch
        }
    }

    void Camera::zoom(float ratio)
    {
        if(ratio == 0.0f)
            return;

        float resize = 1.0f / ratio;
        setFrame(_frame.width() * resize, _frame.height() * resize);

        setLens(_lens.left() * resize,   _lens.right() * resize,
                _lens.bottom() * resize, _lens.top() * resize);
    }

    void Camera::updateMatrices()
    {
        updateProjectionMatrix();
        updateViewMatrix();
    }

    void Camera::updateProjectionMatrix()
    {
        _projMatrix.loadIdentity();

        if(_lens.type() == Lens::EType::ORTHOGRAPHIC)
            _projMatrix *= cellar::ortho(
                _lens.left(),      _lens.right(),
                _lens.bottom(),    _lens.top(),
                _lens.nearPlane(), _lens.farPlane());

        else if(_lens.type() == Lens::EType::PERSPECTIVE)
            _projMatrix *= cellar::frustrum(
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
    Camera::Frame::Frame() :
        _width(1.0f),
        _height(1.0f)
    {
    }

    Camera::Frame::Frame(float width, float height) :
        _width(width),
        _height(height)
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
