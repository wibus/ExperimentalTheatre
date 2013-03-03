#ifndef CELLARWORKBENCH_CAMERA_H
#define CELLARWORKBENCH_CAMERA_H

#include "DesignPattern/SpecificSubject.h"
#include "DataStructure/Vector.h"
#include "DataStructure/Matrix.h"


namespace cellar
{
    struct CameraMsg;

    class CELLAR_EXPORT Camera : public SpecificSubject<CameraMsg>
    {
    public:

        class CELLAR_EXPORT Frame
        {
            friend class Camera;

        public:
            Frame();
            Frame(float width, float height);

            float width() const;
            float height() const;

        private:
            void set(float width, float height);

            float _width;
            float _height;
        };

        class CELLAR_EXPORT Lens
        {            
            friend class Camera;

        public:
            enum Type {ORTHOGRAPHIC, PERSPECTIVE};

            Lens();
            Lens(Type type,
                 double left,       double right,
                 double bottom,     double top,
                 double nearPlane,  double farPlane);

            Type   type() const;
            double left() const;
            double right() const;
            double bottom() const;
            double top() const;
            double nearPlane() const;
            double farPlane() const;
            double width() const;
            double height() const;

        private:
            void set(Type type,
                     double left,         double right,
                     double bottom,       double top,
                     double nearPlane,    double farPlane);

            Type _type;
            double _left;
            double _right;
            double _bottom;
            double _top;
            double _nearPlane;
            double _farPlane;
        };

        class CELLAR_EXPORT Tripod
        {
            friend class Camera;

        public:
            Tripod();
            Tripod(const Vec3f& from,
                   const Vec3f& to,
                   const Vec3f& up);

            const Vec3f& from() const;
            const Vec3f& to() const;
            const Vec3f& up() const;

        private :
            void set(const Vec3f& from,
                     const Vec3f& to,
                     const Vec3f& up);
            void moveBy(const Vec3f& dist);

            Vec3f _from;
            Vec3f _to;
            Vec3f _up;
        };

    public:
        enum Mode {EXPAND, FRAME, STRETCH};

        Camera();
        Camera(Mode mode, const Frame &frame,
               const Lens &lens, const Tripod &tripod,
               int windowWidth, int windowHeight);

        Mode    mode() const;
        void    setMode(Mode mode);

        const Frame&  frame();
        const Lens&   lens();
        const Tripod& tripod();


        // Modifiers
        void setFrame(float width, float height);
        void setLens(double left,         double right,
                     double bottom,       double top);
        void setLens(Lens::Type type,
                     double left,         double right,
                     double bottom,       double top,
                     double nearPlane,    double farPlane);
        void setTripod(const Vec3f& from,
                       const Vec3f& to,
                       const Vec3f& up);
        void moveBy(const Vec3f& dist);
        void refresh();


        // Notify new window size
        void notifyNewWindowSize(int width, int height);
        int  knowWindowWidth() const;
        int  knowWindowHeight() const;

        void zoom(float ratio);

        Mat4f& projectionMatrix();
        Mat4f& viewMatrix();

    private:
        void updateMatrices();
        void updateProjectionMatrix();
        void updateViewMatrix();

        Mode _mode;
        Frame _frame;
        Lens _lens;
        Tripod _tripod;        
        int _windowWidth;
        int _windowHeight;
        Mat4f _projMatrix;
        Mat4f _viewMatrix;
    };

    struct CameraMsg
    {
        enum Change {PROJECTION, VIEW};

        Camera& camera;
        Change change;

    private:
        friend class Camera;
        CameraMsg(Camera& camera, Change change) :
            camera(camera), change(change) {}
    };



    // IMPLEMENTATION //
    // CAMERA //
    inline Camera::Mode Camera::mode() const {return _mode;}
    inline void Camera::setMode(Mode mode) {_mode = mode;}
    inline const Camera::Frame&  Camera::frame() {return _frame;}
    inline const Camera::Lens& Camera::lens() {return _lens;}
    inline const Camera::Tripod &Camera::tripod() {return _tripod;}
    inline int Camera::knowWindowWidth() const {return _windowWidth;}
    inline int Camera::knowWindowHeight() const {return _windowHeight;}
    inline Mat4f& Camera::projectionMatrix() {return _projMatrix;}
    inline Mat4f& Camera::viewMatrix() {return _viewMatrix;}

    // FRAME //
    inline float Camera::Frame::width() const {return _width;}
    inline float Camera::Frame::height() const {return _height;}
    inline void  Camera::Frame::set(float width, float height) {_width = width; _height = height;}

    // CAMERA::LENS //
    inline Camera::Lens::Type Camera::Lens::type() const {return _type;}
    inline double Camera::Lens::left() const {return _left;}
    inline double Camera::Lens::right() const {return _right;}
    inline double Camera::Lens::bottom() const {return _bottom;}
    inline double Camera::Lens::top() const {return _top;}
    inline double Camera::Lens::nearPlane() const {return _nearPlane;}
    inline double Camera::Lens::farPlane() const {return _farPlane;}
    inline double Camera::Lens::width() const {return _right - _left;}
    inline double Camera::Lens::height() const {return _top - _bottom;}


    // CAMERA::TRIPOD //
    inline const Vec3f& Camera::Tripod::from() const {return _from;}
    inline const Vec3f& Camera::Tripod::to() const {return _to;}
    inline const Vec3f& Camera::Tripod::up() const {return _up;}

    inline void Camera::Tripod::set(const Vec3f& from,
                                    const Vec3f& to,
                                    const Vec3f& up)
    {
        _from = from; _to = to; _up = up;
    }

    inline void Camera::Tripod::moveBy(const Vec3f& dist)
        {_from += dist; _to += dist;}
}


#endif // CELLARWORKBENCH_CAMERA_H
