#ifndef SCAENA_QGLWIDGETVIEW_H
#define SCAENA_QGLWIDGETVIEW_H

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

        virtual void draw(double dt,
            const std::function<void()>& customDrawFunc) override;

        virtual glm::ivec2 viewport() const override;

    protected:
        // QGLWidget interface
        virtual void initializeGL() override;
        virtual void resizeGL(int w, int h) override;
        virtual void paintGL() override;

    private:
        std::function<void()> _customDrawFunc;
        double _dt;
    };
}

#endif // SCAENA_QGLWIDGETVIEW_H
