#include "QGlWidgetView.h"

#include <sstream>

#include <GL/glu.h>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QWindow>

#include <CellarWorkbench/Misc/Log.h>

#include <PropRoom2D/Team/AbstractTeam.h>
#include <PropRoom2D/Team/ArtDirector/GlArtDirector.h>

#include <PropRoom3D/Team/AbstractTeam.h>
#include <PropRoom3D/Team/ArtDirector/CpuRaytracerServer.h>

#include "../Play/Play.h"
#include "../StageManagement/Event/MouseEvent.h"
#include "../StageManagement/Event/KeyboardEvent.h"

using namespace cellar;


namespace scaena
{
    namespace
    {
        QGLFormat getGLFormat()
        {
            QGLFormat format(QGL::SampleBuffers);
            format.setSamples(8);
            return format;
        }
    }

    QGlWidgetView::QGlWidgetView(const std::string& id, QWidget* parent) :
        QGLWidget(getGLFormat(), parent),
        View(id)
    {
        setMouseTracking(true);
    }

    QGlWidgetView::~QGlWidgetView()
    {

    }

    glm::ivec2 QGlWidgetView::viewport() const
    {
        return glm::ivec2(width(), height());
    }

    std::shared_ptr<cellar::Camera> QGlWidgetView::camera2D() const
    {
        return _artDirector2D->camera();
    }

    std::shared_ptr<cellar::Camera> QGlWidgetView::camera3D() const
    {
        return _artDirector3D->camera();
    }

    void QGlWidgetView::keyPressEvent(QKeyEvent* event)
    {
        QGLWidget::keyPressEvent(event);
        _keyPressAction(event);
    }

    void QGlWidgetView::keyReleaseEvent(QKeyEvent* event)
    {
        QGLWidget::keyReleaseEvent(event);
        _keyReleaseAction(event);
    }

    void QGlWidgetView::mouseMoveEvent(QMouseEvent* event)
    {
        QGLWidget::mouseMoveEvent(event);
        _mouseMoveAction(event);
    }

    void QGlWidgetView::mousePressEvent(QMouseEvent* event)
    {
        QGLWidget::mousePressEvent(event);
        _mousePressAction(event);
    }

    void QGlWidgetView::mouseReleaseEvent(QMouseEvent* event)
    {
        QGLWidget::mouseReleaseEvent(event);
        _mouseReleaseAction(event);
    }

    void QGlWidgetView::centerOnScreen()
    {
        QPoint center = QApplication::desktop()->availableGeometry(this).center();
        move(center.x()-width()*0.5, center.y()-height()*0.5);
    }

    void QGlWidgetView::setGlWindowSpace(int w, int h)
    {
        resize(w, h);
    }

    // QGLWidget interface
    void QGlWidgetView::initializeGL()
    {
        gl3wInit();

        std::ostringstream oss;
        oss << "OpenGL version: " << glGetString(GL_VERSION);
        getLog().postMessage(new Message('I', false, oss.str(), "QGlWidgetView"));

        oss.str("");
        oss << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
        getLog().postMessage(new Message('I', false, oss.str(), "QGlWidgetView"));

        oss.str("");
        int nbBuffers;
        glGetIntegerv(GL_SAMPLE_BUFFERS, &nbBuffers);
        oss << "Multisampling number of buffers: " << nbBuffers;
        getLog().postMessage(new Message('I', false, oss.str(), "QGlWidgetView"));

        oss.str("");
        int nbSamples;
        glGetIntegerv(GL_SAMPLES, &nbSamples);
        oss << "Multisampling number of samples: " << nbSamples;
        getLog().postMessage(new Message('I', false, oss.str(), "QGlWidgetView"));


        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    void QGlWidgetView::resizeGL(int w, int h)
    {
        glViewport(0, 0, w, h);

        if(_artDirector2D)
            _artDirector2D->resize(w, h);

        if(_artDirector3D)
            _artDirector3D->resize(w, h);
    }

    void QGlWidgetView::paintGL()
    {
        beginDraw(-1);
        endDraw(-1);
    }

    void QGlWidgetView::beginDraw(double dt)
    {
        QWindow* window = windowHandle();
        if(!window || !window->isExposed() || !isVisible())
            return;

        if(dt > 0.0)
        {
            makeCurrent();
        }

        _artDirector3D->draw(glm::max(dt, 0.0));
    }

    void QGlWidgetView::endDraw(double dt)
    {
        QWindow* window = windowHandle();
        if(!window || !window->isExposed() || !isVisible())
            return;

        _artDirector2D->draw(glm::max(dt, 0.0));

        if(dt > 0.0)
        {
            swapBuffers();
        }

        GLenum errCode = glGetError();
        if(errCode)
        {
            const GLubyte* errString = gluErrorString(errCode);
            std::string msg = (const char*) errString;
            msg = "OpenGL error: " + msg;

            getLog().postMessage(
                new Message('E', false, msg, "QGlWidgetView"));
        }
    }

    void QGlWidgetView::setup(Play& play)
    {
        makeCurrent();
        setupArtDirectors(play);
        setupEventListeners(play);
    }

    void QGlWidgetView::setupArtDirectors(Play& play)
    {
        _artDirector2D.reset(new prop2::GlArtDirector());
        play.propTeam2D()->addArtDirector(_artDirector2D);
        _artDirector2D->resize(width(), height());

        _artDirector3D.reset(new prop3::CpuRaytracerServer());
        play.propTeam3D()->addArtDirector(_artDirector3D);
        _artDirector3D->resize(width(), height());
    }

    void QGlWidgetView::setupEventListeners(Play& play)
    {
        _keyPressAction = [&play](QKeyEvent* event)
        {
            if(event->isAutoRepeat())
                return;
            if(play.keyPressEvent(KeyboardEvent::convertQKeyEvent(event)))
                event->accept();
        };

        _keyReleaseAction = [&play](QKeyEvent* event)
        {
            if(event->isAutoRepeat())
                return;
            if(play.keyReleaseEvent(KeyboardEvent::convertQKeyEvent(event)))
                event->accept();
        };

        _mouseMoveAction = [&play](QMouseEvent* event)
        {
            if(play.mouseMoveEvent(MouseEvent::convertQMouseEvent(event)))
                event->accept();
        };

        _mousePressAction = [&play](QMouseEvent* event)
        {
            if(play.mousePressEvent(MouseEvent::convertQMouseEvent(event)))
                event->accept();
        };

        _mouseReleaseAction = [&play](QMouseEvent* event)
        {
            if(play.mouseReleaseEvent(MouseEvent::convertQMouseEvent(event)))
                event->accept();
        };
    }
}
