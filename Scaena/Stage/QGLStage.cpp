#include "QGLStage.h"

#include "Play/AbstractPlay.h"
#include "Act/AbstractAct.h"
#include "Stage/Manager/KeyPressDispatcher.h"
#include "Stage/Manager/KeyReleaseDispatcher.h"
#include "Stage/Manager/MouseMoveDispatcher.h"
#include "Stage/Manager/MousePressDispatcher.h"
#include "Stage/Manager/MouseReleaseDispatcher.h"
#include "Stage/Event/StageTime.h"
#include "Stage/Event/SynchronousKeyboard.h"
#include "Stage/Event/SynchronousMouse.h"

#include <sstream>
using namespace std;

#include <Graphics/Camera/Camera.h>
#include <Graphics/GL/GlToolkit.h>
#include <DateAndTime/QtClock.h>
#include <Misc/Log.h>
using namespace cellar;

#include <PropTeam/StdGlPropTeam.h>
using namespace prop2;

#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>


namespace scaena
{
    QGLStage::QGLStage(QWidget* parent) :
        QGLWidget(parent),
        AbstractStage("QGLStage"),
        _updateTimer(),
        _drawTimer(),
        _isDrawSynch(false)
    {
        setUpdateInterval(15);
        setDrawInterval(0);
        setDrawSynch(true);

        setPropTeam(new StdGlPropTeam());

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
        glewInit();

        ostringstream oss;
        oss << "OpenGL version: " << glGetString(GL_VERSION);
        getLog().postMessage(new Message('I', false, oss.str(), "QGLStage"));

        oss.str("");
        oss << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
        getLog().postMessage(new Message('I', false, oss.str(), "QGLStage"));


        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    void QGLStage::resizeGL(int w, int h)
    {
        glViewport(0,0,w,h);
        camera().notifyNewWindowSize(w, h);
        propTeam().updateViewportSize(w, h);
    }

    void QGLStage::paintGL()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(1.0f, 1.0f, 1.0f);

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
        Keyboard::NonAscii nonAscii = Keyboard::NO_NONASCII;

        if(event->text().toStdString().size() != 0)
            ascii = event->text().toStdString().at(0);

        else
            switch(event->key())
            {
            case Qt::Key_Shift :    nonAscii = Keyboard::SHIFT;   break;
            case Qt::Key_Control :  nonAscii = Keyboard::CTRL;    break;
            case Qt::Key_Tab :      nonAscii = Keyboard::TAB;     break;

            case Qt::Key_Alt :      nonAscii = Keyboard::ALT;     break;
            case Qt::Key_AltGr :    nonAscii = Keyboard::ALT_CAR; break;
            case Qt::Key_Super_L :  nonAscii = Keyboard::SUPER;   break;
            case Qt::Key_Super_R :  nonAscii = Keyboard::SUPER;   break;
            case Qt::Key_Escape :   nonAscii = Keyboard::ESCAPE;  break;

            case Qt::Key_Up :       nonAscii = Keyboard::UP;      break;
            case Qt::Key_Down :     nonAscii = Keyboard::DOWN;    break;
            case Qt::Key_Left :     nonAscii = Keyboard::LEFT;    break;
            case Qt::Key_Right :    nonAscii = Keyboard::RIGHT;   break;

            case Qt::Key_Space :    nonAscii = Keyboard::SPACE;   break;

            default :               nonAscii = Keyboard::NO_NONASCII;
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
        Vector2D<int> pos(event->x(), event->y());
        Vector2D<int> globalPos(event->globalX(), event->globalY());

        Mouse::Button button = Mouse::NO_BUTTON;

        switch(event->button())
        {
        case Qt::LeftButton :   button = Mouse::LEFT;      break;
        case Qt::MiddleButton : button = Mouse::MIDDLE;    break;
        case Qt::RightButton :  button = Mouse::RIGHT;     break;
        default :               button = Mouse::NO_BUTTON;
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
