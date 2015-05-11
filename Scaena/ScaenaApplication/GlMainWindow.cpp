#include "GlMainWindow.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QFileDialog>

#include "../Play/Play.h"
#include "../Play/Act.h"
#include "QGlWidgetView.h"
#include "../StageManagement/Event/MouseEvent.h"
#include "../StageManagement/Event/KeyboardEvent.h"


namespace scaena
{
    GlMainWindow::GlMainWindow(const std::shared_ptr<Play>& play) :
        _play(play),
        _view(nullptr)
    {
        QGlWidgetView* view = new QGlWidgetView("MainView", this);
        _play->addView(std::shared_ptr<View>(view));

        _menuBar = new QMenuBar();
        _fileMenu  = _menuBar->addMenu("File");
        _restartAction = _fileMenu->addAction("Restart");
        _exitAction = _fileMenu->addAction("Exit");

        // Build menu bar
        QObject::connect(_restartAction, &QAction::triggered,
                         [this](){_play->restart();});
        QObject::connect(_exitAction, &QAction::triggered,
                         this,        &GlMainWindow::close);

        QVBoxLayout* layout = new QVBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(_menuBar);
        layout->addWidget(view);

        QWidget* mainWidget = new QWidget();
        mainWidget->setLayout(layout);
        setCentralWidget(mainWidget);

        // Tweaking menu bar size
        _menuBar->setFixedHeight(26);
        _menuBar->setVisible(true);

        // Setting default window size
        setWindowTitle(_play->id().c_str());
        setGlWindowSpace(800, 600);
        setFocus();

        // To listen mouse moves on glwidget
        mainWidget->setMouseTracking(true);
        view->setMouseTracking(true);
        setMouseTracking(true);
    }

    GlMainWindow::GlMainWindow(const std::shared_ptr<Play>& play, QWidget* centralWidget) :
        _play(play)
    {
        setCentralWidget(centralWidget);
        setWindowTitle(_play->id().c_str());

        centralWidget->setMouseTracking(true);
        setMouseTracking(true);

        setGlWindowSpace(800, 600);
        setFocus();
    }

    void GlMainWindow::centerOnScreen()
    {
        QPoint center = QApplication::desktop()->availableGeometry(this).center();
        move(center.x()-width()*0.5, center.y()-height()*0.5);
    }

    void GlMainWindow::setGlWindowSpace(int w, int h)
    {
        resize(w, h + (_menuBar ? _menuBar->height() : 0));
    }

    void GlMainWindow::keyPressEvent(QKeyEvent* event)
    {
        QMainWindow::keyPressEvent(event);

        if(event->isAutoRepeat())
            return;

        if(_play->keyPressEvent(KeyboardEvent::convertQKeyEvent(event)))
        {
            event->accept();
        }
    }

    void GlMainWindow::keyReleaseEvent(QKeyEvent* event)
    {
        QMainWindow::keyReleaseEvent(event);

        if(event->isAutoRepeat())
            return;

        if(_play->keyReleaseEvent(KeyboardEvent::convertQKeyEvent(event)))
        {
            event->accept();
        }
    }

    void GlMainWindow::mousePressEvent(QMouseEvent* event)
    {
        QMainWindow::mousePressEvent(event);

        if(_play->mousePressEvent(MouseEvent::convertQMouseEvent(event)))
        {
            event->accept();
        }
    }

    void GlMainWindow::mouseReleaseEvent(QMouseEvent* event)
    {
        QMainWindow::mouseReleaseEvent(event);

        if(_play->mouseReleaseEvent(MouseEvent::convertQMouseEvent(event)))
        {
            event->accept();
        }
    }

    void GlMainWindow::mouseMoveEvent(QMouseEvent* event)
    {
        QMainWindow::mouseMoveEvent(event);

        if(_play->mouseMoveEvent(MouseEvent::convertQMouseEvent(event)))
        {
            event->accept();
        }
    }
}
