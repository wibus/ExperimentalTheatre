#ifndef PROPROOM3D_QGLPOSTPRODUNIT_H
#define PROPROOM3D_QGLPOSTPRODUNIT_H

#include <GLM/glm.hpp>

#include <QWidget>
#include <QTableWidget>

#include <MediaWorkbench/GL/GlProgram.h>

#include "../../libPropRoom3D_global.h"

namespace Ui
{
    class QGlPostProdUnit;
}


namespace prop3
{
    class PROP3D_EXPORT QGlPostProdUnit : public QWidget
    {
        Q_OBJECT

    public:
        QGlPostProdUnit();
        virtual ~QGlPostProdUnit();

        virtual void setColorBufferTexId(unsigned int id);

        virtual void setup();
        virtual void execute();


    private slots:
        virtual void activateLowPassChecked(int state);
        virtual void lowpassSizeChanged(int sizeIndex);
        virtual void lowpassVarianceChanged(double variance);
        virtual void useAdaptativeFilteringChecked(int state);
        virtual void adaptativeFilteringFactorChanged(int factor);
        virtual void temperatureChanged(int kelvin);
        virtual void temperatureDefaultClicked();
        virtual void contrastChanged(int contrast);
        virtual void luminosityChanged(int luminosity);
        virtual void saveOutputImage();

    private:
        virtual void updateKernel(double variance, int size);

        static glm::vec3 kelvinToRgb(int kelvin);
        static float computeLuminosityFactor(int luminosity);
        static float computeContrastFactor(int contrast);
        static float computeAdaptativeFactor(int factor);
        static void buildLowpassKernel(float kernel[], double variance, int size);
        static void updateLowpassKernelTable(QTableWidget* widget, float kernel[]);
        static void updateLowpassKernelUniform(media::GlProgram& prog, float kernel[], int size);


    private:
        std::shared_ptr<Ui::QGlPostProdUnit> _ui;
        unsigned int _colorBufferTexId;

        media::GlProgram _postProdProgram;
        unsigned int _fullscreenVao;
        unsigned int _fullscreenVbo;

        glm::vec3 _temperatureColor;
        float _luminosityValue;
        float _contrastValue;
        float _lowpassKernel[25];
        int _lowpassKernelSize;
        double _lowpassKernelVar;
        unsigned int _filteringFunc;
        float _adaptationFactor;

        static const int DEFAULT_WHITE_TEMPERATURE;
    };
}

#endif // PROPROOM3D_QGLPOSTPRODUNIT_H
