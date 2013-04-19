#ifndef SCAENA_GLMAINWINDOW_H
#define SCAENA_GLMAINWINDOW_H

#include <QWidget>
#include <Stage/QGLStage.h>

class QMenuBar;
class QMenu;
class QAction;


namespace scaena
{
    class SCAENA_EXPORT GlMainWindow : public QWidget
    {
        Q_OBJECT

    public:
        GlMainWindow(scaena::QGLStage* stage);

        void centerOnScreen();
        void setGlWindowSpace(int w, int h);

    public slots:

    signals:

    private:
        scaena::QGLStage* _stage;
        QMenuBar* _menuBar;
        QMenu*    _fileMenu;
        QAction*  _restartAction;
        QAction*  _exitAction;
    };
}

#endif // SCAENA_GLMAINWINDOW_H
