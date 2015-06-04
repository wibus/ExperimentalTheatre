#ifndef SCAENA_QGLWIDGETVIEW_H
#define SCAENA_QGLWIDGETVIEW_H

#include <functional>

#include <GL3/gl3w.h>

#include <QGLWidget>

#include "../Play/View.h"


namespace scaena
{
    class SCAENA_EXPORT QGlWidgetView :
            public QGLWidget,
            public View
    {
        Q_OBJECT

    public:
        QGlWidgetView(const std::string& id, QWidget* parent=nullptr);
        virtual ~QGlWidgetView();

        virtual glm::ivec2 viewport() const override;
        virtual std::shared_ptr<cellar::Camera> camera2D() const override;
        virtual std::shared_ptr<cellar::Camera> camera3D() const override;

        virtual void keyPressEvent(QKeyEvent* event) override;
        virtual void keyReleaseEvent(QKeyEvent* event) override;
        virtual void mousePressEvent(QMouseEvent* event) override;
        virtual void mouseReleaseEvent(QMouseEvent* event) override;
        virtual void mouseMoveEvent(QMouseEvent* event) override;
        virtual void wheelEvent(QWheelEvent* event) override;

        virtual void centerOnScreen();
        virtual void setGlWindowSpace(int w, int h);

    protected:
        // QGLWidget interface
        virtual void initializeGL() override;
        virtual void resizeGL(int w, int h) override;
        virtual void paintGL() override;

        // View interface
        virtual void beginDraw(double dt)  override;
        virtual void endDraw(double dt)  override;
        virtual void setup(Play& play)  override;
        virtual void setupArtDirectors(Play& play);
        virtual void setupEventListeners(Play& play);


    protected:
        std::shared_ptr<prop2::AbstractArtDirector> _artDirector2D;
        std::shared_ptr<prop3::AbstractArtDirector> _artDirector3D;

    private:
        std::function<void(QKeyEvent*)> _keyPressAction;
        std::function<void(QKeyEvent*)> _keyReleaseAction;
        std::function<void(QMouseEvent*)> _mouseMoveAction;
        std::function<void(QMouseEvent*)> _mousePressAction;
        std::function<void(QMouseEvent*)> _mouseReleaseAction;
        std::function<void(QWheelEvent*)> _mouseWheelAction;
    };
}

#endif // SCAENA_QGLWIDGETVIEW_H
