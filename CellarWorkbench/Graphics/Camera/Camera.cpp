#include "Camera.h"


namespace cellar
{
    // CAMERA //
    Camera::Camera() :
        _mode(EXPAND),
        _frame(),
        _lens(),
        _tripod(),
        _windowWidth(2),
        _windowHeight(2),
        _projMatrix(),
        _viewMatrix()
    {
        updateMatrices();
    }

    Camera::Camera(Mode mode, const Frame& frame,
                   const Lens &lens, const Tripod &tripod,
                   int windowWidth,  int windowHeight) :
        _mode(mode),
        _frame(frame),
        _lens(lens),
        _tripod(tripod),
        _windowWidth(windowWidth),
        _windowHeight(windowHeight),
        _projMatrix(),
        _viewMatrix()
    {
        updateMatrices();
    }

    void Camera::setFrame(float width, float height)
    {
        _frame.set(width, height);
    }

    void Camera::setLens(double left,         double right,
                         double bottom,       double top)
    {
        setLens(_lens.type(),
                left,              right,
                bottom,            top,
                _lens.nearPlane(), _lens.farPlane());
    }

    void Camera::setLens(Lens::Type type,
                         double left,         double right,
                         double bottom,       double top,
                         double nearPlane,    double farPlane)
    {
        _lens.set(type, left, right, bottom, top, nearPlane, farPlane);
        updateProjectionMatrix();
    }

    void Camera::setTripod(const Vec3f& from,
                           const Vec3f& to,
                           const Vec3f& up)
    {
        _tripod.set(from, to, up);
        updateViewMatrix();
    }

    void Camera::moveBy(const Vec3f& dist)
    {
        _tripod.moveBy( dist );
        updateViewMatrix();
    }

    void Camera::refresh()
    {
        updateMatrices();
    }

    void Camera::notifyNewWindowSize(int width, int height)
    {
        if(width == 0 || height == 0)
            return;

        if(_mode == EXPAND)
        {
            float wRatio = static_cast<float>(width) / _windowWidth;
            float hRatio = static_cast<float>(height) / _windowHeight;

            setLens(_lens.left() * wRatio,   _lens.right() * wRatio,
                    _lens.bottom() * hRatio, _lens.top() * hRatio);

            setFrame(_lens.right() - _lens.left(),
                     _lens.top() - _lens.bottom());
        }
        else if(_mode == FRAME)
        {
            float aspectRatio = static_cast<float>(width) / height;
            float dW, dH;

            if( aspectRatio < (_frame.width() / _frame.height()) )
            {
                dW = _frame.width() / 2.0f;
                dH = dW / aspectRatio;
            }
            else
            {
                dH = _frame.height() / 2.0f;
                dW = dH * aspectRatio;
            }
            setLens(-dW, dW,
                    -dH, dH);
        }
        else if(_mode == STRETCH)
        {
            // Let stretch
        }

        _windowWidth = width;
        _windowHeight = height;
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

        if(_lens.type() == Lens::ORTHOGRAPHIC)
            _projMatrix *= ortho(_lens.left(),       _lens.right(),
                                 _lens.bottom(),     _lens.top(),
                                 _lens.nearPlane(),  _lens.farPlane());

        else if(_lens.type() == Lens::PERSPECTIVE)
            _projMatrix *= frustrum(_lens.left(),      _lens.right(),
                                    _lens.bottom(),    _lens.top(),
                                    _lens.nearPlane(), _lens.farPlane());

        setIsChanged( true );
        CameraMsg msg(*this, CameraMsg::PROJECTION);
        notifyObservers( msg );
    }

    void Camera::updateViewMatrix()
    {
        _viewMatrix = lookAt(_tripod.from(), _tripod.to(), _tripod.up());

        setIsChanged( true );
        CameraMsg msg(*this, CameraMsg::VIEW);
        notifyObservers( msg);
    }


    // CAMERA::FRAME //
    Camera::Frame::Frame() :
        _width(2.0f),
        _height(2.0f)
    {
    }

    Camera::Frame::Frame(float width, float height) :
        _width(width),
        _height(height)
    {
    }

    // CAMERA::LENS //
    Camera::Lens::Lens() :
        _type(ORTHOGRAPHIC),
        _left(-1.0),
        _right(1.0),
        _bottom(-1.0),
        _top(1.0),
        _nearPlane(-1.0),
        _farPlane(1.0)
    {
    }

    Camera::Lens::Lens(Type type,
         double left,       double right,
         double bottom,     double top,
         double nearPlane,  double farPlane) :
        _type(type),
        _left(left),
        _right(right),
        _bottom(bottom),
        _top(top),
        _nearPlane(nearPlane),
        _farPlane(farPlane)
    {
    }

    void Camera::Lens::set(Type type,
                           double left,      double right,
                           double bottom,    double top,
                           double nearPlane, double farPlane)
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

    Camera::Tripod::Tripod(const Vec3f& from,
                           const Vec3f& to,
                           const Vec3f& up) :
        _from(from),
        _to(to),
        _up(up)
    {}
}
