#ifndef MEDIAWORKBENCH_CAMERA_H
#define MEDIAWORKBENCH_CAMERA_H

#include <CellarWorkbench/DesignPattern/SpecificSubject.h>
#include <CellarWorkbench/DataStructure/Vector.h>
#include <CellarWorkbench/DataStructure/Matrix.h>

#include "../libMediaWorkbench_global.h"


namespace media
{
    struct CameraMsg;

    class MEDIA_EXPORT Camera : public cellar::SpecificSubject<CameraMsg>
    {
    public:

        class MEDIA_EXPORT Frame
        {
            friend class Camera;

        public:
            explicit Frame(const cellar::Vec2f& size = cellar::Vec2f(1, 1),
                           const cellar::Vec2f& center = cellar::Vec2f(0, 0));

            const cellar::Vec2f& size() const;
            const cellar::Vec2f& center() const;
            float aspectRatio() const;

        private:
            void set(const cellar::Vec2f& size,
                     const cellar::Vec2f& center);

            cellar::Vec2f _size;
            cellar::Vec2f _center;
        };

        class MEDIA_EXPORT Lens
        {            
            friend class Camera;

        public:
            enum class EType {ORTHOGRAPHIC, PERSPECTIVE};

            Lens();
            Lens(EType type,
                 float left,       float right,
                 float bottom,     float top,
                 float nearPlane,  float farPlane);

            EType type() const;
            float left() const;
            float right() const;
            float bottom() const;
            float top() const;
            float nearPlane() const;
            float farPlane() const;
            float width() const;
            float height() const;

        private:
            void set(EType type,
                     float left,         float right,
                     float bottom,       float top,
                     float nearPlane,    float farPlane);

            EType _type;
            float _left;
            float _right;
            float _bottom;
            float _top;
            float _nearPlane;
            float _farPlane;
        };

        class MEDIA_EXPORT Tripod
        {
            friend class Camera;

        public:
            Tripod();
            Tripod(const cellar::Vec3f& from,
                   const cellar::Vec3f& to,
                   const cellar::Vec3f& up);

            const cellar::Vec3f& from() const;
            const cellar::Vec3f& to() const;
            const cellar::Vec3f& up() const;

        private :
            void set(const cellar::Vec3f& from,
                     const cellar::Vec3f& to,
                     const cellar::Vec3f& up);
            void moveBy(const cellar::Vec3f& dist);

            cellar::Vec3f _from;
            cellar::Vec3f _to;
            cellar::Vec3f _up;
        };

    public:
        enum class EMode {EXPAND, FRAME, STRETCH};

        Camera();
        Camera(EMode mode, const Frame &frame,
               const Lens &lens, const Tripod &tripod,
               const cellar::Vec2i& viewport);

        EMode    mode() const;
        void    setMode(EMode mode);

        const Frame&  frame() const;
        const Lens&   lens() const;
        const Tripod& tripod() const;


        // Modifiers
        void setFrame(const cellar::Vec2f& size);
        void setFrame(const cellar::Vec2f& size,
                      const cellar::Vec2f& center);
        void setLens(Lens::EType type);
        void setLens(float nearPlane,    float farPlane);
        void setLens(float left,         float right,
                     float bottom,       float top);
        void setLens(Lens::EType type,
                     float left,         float right,
                     float bottom,       float top,
                     float nearPlane,    float farPlane);
        void setTripod(const cellar::Vec3f& from,
                       const cellar::Vec3f& to,
                       const cellar::Vec3f& up);
        void moveBy(const cellar::Vec3f& dist);

        void refresh();
        void refreshProjection();


        // Notify new window size
        void updateViewport(int width, int height);
        const cellar::Vec2i& viewport() const;
        int  viewportWidth() const;
        int  viewportHeight() const;

        const cellar::Mat4f& projectionMatrix() const;
        const cellar::Mat4f& viewMatrix() const;

    private:
        void updateMatrices();
        void updateProjectionMatrix();
        void updateViewMatrix();

        EMode _mode;
        Frame _frame;
        Lens _lens;
        Tripod _tripod;
        cellar::Vec2i _viewport;
        cellar::Mat4f _projMatrix;
        cellar::Mat4f _viewMatrix;
    };

    struct CameraMsg
    {
        enum class EChange {PROJECTION, VIEW};

        Camera& camera;
        EChange change;

    private:
        friend class Camera;
        CameraMsg(Camera& camera, EChange change) :
            camera(camera), change(change) {}
    };



    // IMPLEMENTATION //
    // CAMERA //
    inline Camera::EMode Camera::mode() const {return _mode;}
    inline const Camera::Frame&  Camera::frame() const {return _frame;}
    inline const Camera::Lens& Camera::lens() const {return _lens;}
    inline const Camera::Tripod &Camera::tripod() const {return _tripod;}
    inline const cellar::Vec2i& Camera::viewport() const {return _viewport;}
    inline int Camera::viewportWidth() const {return _viewport.x();}
    inline int Camera::viewportHeight() const {return _viewport.y();}
    inline const cellar::Mat4f& Camera::projectionMatrix() const {return _projMatrix;}
    inline const cellar::Mat4f& Camera::viewMatrix() const {return _viewMatrix;}

    // FRAME //
    inline const cellar::Vec2f& Camera::Frame::size() const {return _size;}
    inline const cellar::Vec2f& Camera::Frame::center() const {return _center;}
    inline float Camera::Frame::aspectRatio() const {return _size.x() / _size.y();}
    inline void  Camera::Frame::set(const cellar::Vec2f& size,
                                    const cellar::Vec2f& center)
    {
        _size = size;
        _center = center;
    }

    // CAMERA::LENS //
    inline Camera::Lens::EType Camera::Lens::type() const {return _type;}
    inline float Camera::Lens::left() const {return _left;}
    inline float Camera::Lens::right() const {return _right;}
    inline float Camera::Lens::bottom() const {return _bottom;}
    inline float Camera::Lens::top() const {return _top;}
    inline float Camera::Lens::nearPlane() const {return _nearPlane;}
    inline float Camera::Lens::farPlane() const {return _farPlane;}
    inline float Camera::Lens::width() const {return _right - _left;}
    inline float Camera::Lens::height() const {return _top - _bottom;}


    // CAMERA::TRIPOD //
    inline const cellar::Vec3f& Camera::Tripod::from() const {return _from;}
    inline const cellar::Vec3f& Camera::Tripod::to() const {return _to;}
    inline const cellar::Vec3f& Camera::Tripod::up() const {return _up;}

    inline void Camera::Tripod::set(const cellar::Vec3f& from,
                                    const cellar::Vec3f& to,
                                    const cellar::Vec3f& up)
    {
        _from = from;
        _to = to;
        _up = up;
    }

    inline void Camera::Tripod::moveBy(const cellar::Vec3f& dist)
    {
        _from += dist;
        _to += dist;
    }
}


#endif // MEDIAWORKBENCH_CAMERA_H
