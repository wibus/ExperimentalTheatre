#ifndef PROPROOM3D_GLPOSTPRODUNIT_H
#define PROPROOM3D_GLPOSTPRODUNIT_H

#include <GLM/glm.hpp>

#include <CellarWorkbench/GL/GlProgram.h>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    enum class KernelSize {SIZE_3x3, SIZE_5x5};

    class PROP3D_EXPORT GlPostProdUnit
    {
    public:
        GlPostProdUnit();
        virtual ~GlPostProdUnit();

        virtual const float* lowpassKernel() const;

        virtual void setColorBufferTexId(unsigned int id);

        virtual void setup();
        virtual void execute();

        virtual void activateLowPassFilter(bool activate);
        virtual void enableAdaptativeFiltering(bool enable);
        virtual void setLowpassKernelSize(KernelSize size);
        virtual void setLowpassVariance(double variance);
        virtual void setAdaptativeFilteringFactor(float zeroToOne);
        virtual void setImageTemperature(int kelvin);
        virtual void setImageContrast(float minusOneToOne);
        virtual void setImageLuminosity(float zeroToOne);
        virtual void saveOutputImage();

        static const int DEFAULT_WHITE_TEMPERATURE;


    private:
        static glm::vec3 kelvinToRgb(int kelvin);
        virtual void updateKernel(double variance, int size);
        static void buildLowpassKernel(float kernel[], double variance, int size);
        static void updateLowpassKernelUniform(cellar::GlProgram& prog, float kernel[], int size);


    private:
        unsigned int _colorBufferTexId;

        cellar::GlProgram _postProdProgram;
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
        bool _adaptationEnabled;
    };
}

#endif // PROPROOM3D_GLPOSTPRODUNIT_H
