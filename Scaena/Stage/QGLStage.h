#ifndef SCAENA_QGLSTAGE_H
#define SCAENA_QGLSTAGE_H

#include <GL3/gl3w.h>

#include <QGLWidget>
#include <QTimer>
#include <QElapsedTimer>

#include "Stage/AbstractStage.h"


namespace scaena
{
    class KeyboardEvent;
    class MouseEvent;

    class SCAENA_EXPORT QGLStage :
            public QGLWidget,
            public AbstractStage
    {
        Q_OBJECT

    public:
        QGLStage(QWidget* parent=0);
        virtual ~QGLStage();

        // Stage interface
        virtual void mainLoop();
        virtual void terminate();

        virtual void show();
        virtual void setSize(int w, int h);
        virtual void setWidth(int w);
        virtual int  width() const;
        virtual void setHeight(int h);
        virtual int  height() const;
        virtual void setFullScreen(bool full);

        virtual int  updateInterval() const;
        virtual int  drawInterval() const;
        virtual bool isDrawSynch() const;

        virtual void setUpdateInterval(int interval);
        virtual void setDrawInterval(int interval);
        virtual void setDrawSynch(bool isSynch);


        virtual void keyPressEvent(QKeyEvent* event);
        virtual void keyReleaseEvent(QKeyEvent* event);
        virtual KeyboardEvent convertKeyEvent(QKeyEvent* event);

        virtual void mousePressEvent(QMouseEvent* event);
        virtual void mouseReleaseEvent(QMouseEvent* event);
        virtual void mouseMoveEvent(QMouseEvent* event);
        virtual MouseEvent convertMouseEvent(QMouseEvent* event);

    public slots:
        virtual void restartPlay();
        virtual void restartAct();

    protected:
        // for subclassing
        QGLStage(const std::string& id, QWidget* parent=0);

        // QGLWidget interface
        virtual void initializeGL();
        virtual void resizeGL(int w, int h);
        virtual void paintGL();


    protected slots:
        virtual void update();

    private:
        QTimer _updateTimer;
        QTimer _drawTimer;
        bool   _isDrawSynch;
    };
}

#endif // SCAENA_QGLSTAGE_H
