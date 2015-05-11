#ifndef SCAENA_GLMAINWINDOW_H
#define SCAENA_GLMAINWINDOW_H

#include <memory>

#include <QMainWindow>
class QMenuBar;
class QMenu;
class QAction;

#include "../libScaena_global.h"


namespace scaena
{
    class Play;
    class View;


    class SCAENA_EXPORT GlMainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        GlMainWindow(const std::shared_ptr<Play>& play);
        GlMainWindow(const std::shared_ptr<Play>& play, QWidget* centralWidget);

        void centerOnScreen();
        void setGlWindowSpace(int w, int h);

        virtual void keyPressEvent(QKeyEvent* event);
        virtual void keyReleaseEvent(QKeyEvent* event);
        virtual void mousePressEvent(QMouseEvent* event);
        virtual void mouseReleaseEvent(QMouseEvent* event);
        virtual void mouseMoveEvent(QMouseEvent* event);

    private:
        std::shared_ptr<Play> _play;
        std::shared_ptr<View> _view;
        QMenuBar* _menuBar;
        QMenu*    _fileMenu;
        QAction*  _restartAction;
        QAction*  _exitAction;
    };
}

#endif // SCAENA_GLMAINWINDOW_H
