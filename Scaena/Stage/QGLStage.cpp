#include "QGLStage.h"

#include <sstream>
using namespace std;

#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>

#include <CellarWorkbench/DateAndTime/QtClock.h>
#include <CellarWorkbench/Misc/Log.h>
using namespace cellar;

#include <MediaWorkbench/Camera/Camera.h>
#include <MediaWorkbench/GL/GlToolkit.h>
using namespace media;

#include <PropRoom2D/PropTeam/StdGlPropTeam.h>
#include <PropRoom3D/Team/StdGlTeam.h>
using namespace prop2;
using namespace prop3;

#include "../Play/AbstractPlay.h"
#include "../Act/AbstractAct.h"
#include "Manager/KeyPressDispatcher.h"
#include "Manager/KeyReleaseDispatcher.h"
#include "Manager/MouseMoveDispatcher.h"
#include "Manager/MousePressDispatcher.h"
#include "Manager/MouseReleaseDispatcher.h"
#include "Event/StageTime.h"
#include "Event/SynchronousKeyboard.h"
#include "Event/SynchronousMouse.h"


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

    QGLStage::QGLStage(QWidget* parent) :
        QGLWidget(getGLFormat() ,parent),
        AbstractStage("QGLStage"),
        _updateTimer(),
        _drawTimer(),
        _isDrawSynch(false)
    {
        setUpdateInterval(20);
        setDrawInterval(0);
        setDrawSynch(true);

        setPropTeam2D(new prop2::StdGlPropTeam());
        setPropTeam3D(new prop3::StdGlTeam());

        _updateClock.reset( new QtClock() );
        _drawClock.reset( new QtClock() );

        connect(&_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
        connect(&_drawTimer,   SIGNAL(timeout()), this, SLOT(updateGL()));
    }

    QGLStage::QGLStage(const std::string& id, QWidget* parent) :
        QGLWidget(parent),
        AbstractStage(id)
    {}

    QGLStage::~QGLStage()
    {
    }


    // Stage interface
    void QGLStage::mainLoop()
    {
        // Events tweaking
        setMouseTracking( true );
        setFocus();
        QWidget::setWindowTitle( QString(play().id().c_str()) );

        startPlay();
        setAsRunning(true);

        // Clocks
        _updateClock->start();
        _drawClock->start();

        // Timers
        _updateTimer.start();
        if(!isDrawSynch())
            _drawTimer.start();

        QApplication::exec();

        endPlay();
    }

    void QGLStage::terminate()
    {
        _isRunning = false;
        close();
    }

    void QGLStage::show()
    {
        QGLWidget::show();

        QRect frect = frameGeometry();
        frect.moveCenter(QDesktopWidget().availableGeometry().center());
        move(frect.topLeft());
    }

    void QGLStage::setSize(int w, int h)
    {
        QGLWidget::resize(w,h);
    }

    void QGLStage::setWidth(int w)
    {
        QGLWidget::resize(w, height());
    }

    int QGLStage::width() const
    {
        return QGLWidget::width();
    }

    void QGLStage::setHeight(int h)
    {
        QGLWidget::resize(width(), h);
    }

    int QGLStage::height() const
    {
        return QGLWidget::height();
    }

    void QGLStage::setFullScreen(bool full)
    {
        if(full)
            QGLWidget::showFullScreen();
        // else
            QGLWidget::showNormal();
    }


    // Timers
    int QGLStage::updateInterval() const
    {
        return _updateTimer.interval();
    }

    int QGLStage::drawInterval() const
    {
        return _drawTimer.interval();
    }

    bool QGLStage::isDrawSynch() const
    {
        return _isDrawSynch;
    }

    void QGLStage::setUpdateInterval(int interval)
    {
        _updateTimer.setInterval(interval);
    }

    void QGLStage::setDrawInterval(int interval)
    {
        _drawTimer.setInterval(interval);
    }

    void QGLStage::setDrawSynch(bool isSynch)
    {
        if(isSynch)
        {
            if(!_isDrawSynch)
                connect(&_updateTimer, SIGNAL(timeout()),
                        this,          SLOT(updateGL()));
            if(isRunning())
                _drawTimer.stop();
        }
        else
        {
            if(_isDrawSynch)
                disconnect(&_updateTimer, SIGNAL(timeout()),
                           this,          SLOT(updateGL()));
            if(isRunning())
                _drawTimer.start();
        }        

        _isDrawSynch = isSynch;
    }


    // QGLWidget interface
    void QGLStage::initializeGL()
    {
        gl3wInit();

        ostringstream oss;
        oss << "OpenGL version: " << glGetString(GL_VERSION);
        getLog().postMessage(new Message('I', false, oss.str(), "QGLStage"));

        oss.str("");
        oss << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
        getLog().postMessage(new Message('I', false, oss.str(), "QGLStage"));

        oss.str("");
        int nbBuffers;
        glGetIntegerv(GL_SAMPLE_BUFFERS, &nbBuffers);
        oss << "Multisampling number of buffers: " << nbBuffers;
        getLog().postMessage(new Message('I', false, oss.str(), "QGLStage"));

        oss.str("");
        int nbSamples;
        glGetIntegerv(GL_SAMPLES, &nbSamples);
        oss << "Multisampling number of samples: " << nbSamples;
        getLog().postMessage(new Message('I', false, oss.str(), "QGLStage"));


        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    void QGLStage::resizeGL(int w, int h)
    {
        glViewport(0,0,w,h);
        _propTeam2D->resize(w, h);

        // TODO : PropTeam3D own camera
        camera().updateViewport(w, h);
    }

    void QGLStage::paintGL()
    {
        draw();
    }

    void QGLStage::update()
    {
        AbstractStage::update();
    }

    void QGLStage::keyPressEvent(QKeyEvent * event)
    {
        QGLWidget::keyPressEvent(event);

        if(event->isAutoRepeat())
            return;

        KeyboardEvent kEvent= convertKeyEvent(event);
        _synchronousKeyboard->setIsAsciiPressed(kEvent.getAscii(), true);
        _synchronousKeyboard->setIsNonAsciiPressed(kEvent.getNonAscii(), true);
        KeyPressDispatcher kpVisitor( kEvent );
        play().welcome( kpVisitor );
    }

    void QGLStage::keyReleaseEvent(QKeyEvent * event)
    {
        QGLWidget::keyReleaseEvent( event );

        if(event->isAutoRepeat())
            return;

        KeyboardEvent kEvent= convertKeyEvent(event);
        _synchronousKeyboard->setIsAsciiPressed(kEvent.getAscii(), false);
        _synchronousKeyboard->setIsNonAsciiPressed(kEvent.getNonAscii(), false);
        KeyReleaseDispatcher krVisitor( kEvent );
        play().welcome( krVisitor );
    }

    KeyboardEvent QGLStage::convertKeyEvent(QKeyEvent * event)
    {
        char ascii = '\0';
        ENonAscii nonAscii = ENonAscii::NO_NONASCII;

        if(event->text().toStdString().size() != 0)
            ascii = event->text().toStdString().at(0);

        else
            switch(event->key())
            {
            case Qt::Key_Shift :    nonAscii = ENonAscii::SHIFT;   break;
            case Qt::Key_Control :  nonAscii = ENonAscii::CTRL;    break;
            case Qt::Key_Tab :      nonAscii = ENonAscii::TAB;     break;

            case Qt::Key_Alt :      nonAscii = ENonAscii::ALT;     break;
            case Qt::Key_AltGr :    nonAscii = ENonAscii::ALT_CAR; break;
            case Qt::Key_Super_L :  nonAscii = ENonAscii::SUPER;   break;
            case Qt::Key_Super_R :  nonAscii = ENonAscii::SUPER;   break;
            case Qt::Key_Escape :   nonAscii = ENonAscii::ESCAPE;  break;

            case Qt::Key_Up :       nonAscii = ENonAscii::UP;      break;
            case Qt::Key_Down :     nonAscii = ENonAscii::DOWN;    break;
            case Qt::Key_Left :     nonAscii = ENonAscii::LEFT;    break;
            case Qt::Key_Right :    nonAscii = ENonAscii::RIGHT;   break;

            case Qt::Key_Space :    nonAscii = ENonAscii::SPACE;   break;

            default :               nonAscii = ENonAscii::NO_NONASCII;
            }

        return KeyboardEvent(ascii, nonAscii);
    }

    void QGLStage::mousePressEvent(QMouseEvent* event)
    {
        QGLWidget::mousePressEvent(event);

        MouseEvent mEvent = convertMouseEvent(event);
        _synchronousMouse->setButtonIsPressed(mEvent.button(), true);
        MousePressDispatcher mpVisitor( mEvent );
        play().welcome( mpVisitor );
    }

    void QGLStage::mouseReleaseEvent(QMouseEvent* event)
    {
        QGLWidget::mouseReleaseEvent(event);

        MouseEvent mEvent = convertMouseEvent(event);
        _synchronousMouse->setButtonIsPressed(mEvent.button(), false);
        MouseReleaseDispatcher mrVisitor( mEvent );
        play().welcome( mrVisitor );
    }

    void QGLStage::mouseMoveEvent(QMouseEvent* event)
    {
        QGLWidget::mouseMoveEvent(event);

        MouseEvent mEvent = convertMouseEvent(event);
        _synchronousMouse->setPosition( mEvent.position() );
        MouseMoveDispatcher mmVisitor( mEvent );
        play().welcome( mmVisitor );
    }

    MouseEvent QGLStage::convertMouseEvent(QMouseEvent* event)
    {
        glm::ivec2 pos(event->x(), event->y());
        glm::ivec2 globalPos(event->globalX(), event->globalY());

        EMouseButton button = EMouseButton::NO_BUTTON;

        switch(event->button())
        {
        case Qt::LeftButton :   button = EMouseButton::LEFT;      break;
        case Qt::MiddleButton : button = EMouseButton::MIDDLE;    break;
        case Qt::RightButton :  button = EMouseButton::RIGHT;     break;
        default :               button = EMouseButton::NO_BUTTON;
        }

        return MouseEvent(button, pos, globalPos);
    }

    void QGLStage::restartPlay()
    {
        _play->restart();
    }

    void QGLStage::restartAct()
    {
        _play->currentAct().restart();
    }
}
