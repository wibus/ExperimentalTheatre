#include "QGlWidgetView.h"

#include <sstream>

#include <CellarWorkbench/Misc/Log.h>

#include <PropRoom2D/Team/ArtDirector/GlArtDirector.h>
#include <PropRoom3D/Team/ArtDirector/CpuRaytracerServer.h>

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
        _dt = 0;
        _customDrawFunc = [](){};

        setArtDirector2D(std::shared_ptr<prop2::AbstractArtDirector>(
            new prop2::GlArtDirector()));

        setArtDirector3D(std::shared_ptr<prop3::AbstractArtDirector>(
            new prop3::CpuRaytracerServer()));
    }

    QGlWidgetView::~QGlWidgetView()
    {

    }

    void QGlWidgetView::draw(double dt, const std::function<void()>& customDrawFunc)
    {
        _dt = dt;
        _customDrawFunc = customDrawFunc;
        updateGL();
    }

    glm::ivec2 QGlWidgetView::viewport() const
    {
        return glm::ivec2(width(), height());
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
        glViewport(0,0,w,h);
        artDirector2D()->resize(w, h);
        artDirector3D()->resize(w, h);
    }

    void QGlWidgetView::paintGL()
    {
        artDirector3D()->draw(_dt);
        _customDrawFunc();
        artDirector2D()->draw(_dt);
    }
}
