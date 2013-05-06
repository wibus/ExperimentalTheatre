#include "GlMainWindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QFileDialog>

#include <Play/AbstractPlay.h>
#include <Act/AbstractAct.h>


namespace scaena
{
    GlMainWindow::GlMainWindow(scaena::QGLStage *stage) :
        _stage(stage),
        _menuBar(new QMenuBar())
    {
        QVBoxLayout* layout = new QVBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);

        // Build menu bar
        _fileMenu  = _menuBar->addMenu("File");

        _restartAction = _fileMenu->addAction("Restart");
        QObject::connect(_restartAction, SIGNAL(triggered()),
                         _stage,         SLOT(restartPlay()));

        _exitAction = _fileMenu->addAction("Exit");
        QObject::connect(_exitAction, SIGNAL(triggered()),
                         this,        SLOT(close()));


        // Tweak layout
        _menuBar->setFixedHeight(26);
        layout->addWidget(_menuBar);
        layout->addWidget(_stage);
        setLayout(layout);

        resize(800, 600 + _menuBar->height());
    }

    void GlMainWindow::centerOnScreen()
    {
        QPoint center = QApplication::desktop()->availableGeometry(this).center();
        move(center.x()-width()*0.5, center.y()-height()*0.5);
    }

    void GlMainWindow::setGlWindowSpace(int w, int h)
    {
        resize(w, h + _menuBar->height());
    }
}
