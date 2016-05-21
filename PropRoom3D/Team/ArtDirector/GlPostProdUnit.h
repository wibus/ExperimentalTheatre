#ifndef PROPROOM3D_GLPOSTPRODUNIT_H
#define PROPROOM3D_GLPOSTPRODUNIT_H

#include <GLM/glm.hpp>

#include <CellarWorkbench/GL/GlProgram.h>

#include "Film/Film.h"


namespace prop3
{
    enum class KernelSize {SIZE_3x3, SIZE_5x5};

    class PROP3D_EXPORT GlPostProdUnit
    {
    public:
        GlPostProdUnit();
        virtual ~GlPostProdUnit();

        virtual const float* lowpassKernel() const;

        virtual void updateDepthRange(const glm::vec2& range);

        virtual void setup();
        virtual void execute();
        virtual void clearOutput();
        virtual void update(Film& film,
            Film::ColorOutput& colorOutput);

        virtual void activateLowPassFilter(bool activate);
        virtual void activateAdaptativeFiltering(bool enable);
        virtual void setLowpassKernelSize(KernelSize size);
        virtual void setLowpassVariance(double variance);
        virtual void setAdaptativeFilteringFactor(float zeroToOne);
        virtual void setImageTemperature(int kelvin);
        virtual void setImageContrast(float minusOneToOne);
        virtual void setImageLuminosity(float zeroToOne);
        virtual void saveOutputImage();

        virtual void fetchImageMinAndMax(
                glm::dvec3& minComp,
                glm::dvec3& maxComp);

        virtual void getEqualizedImage(
                double& luminosity,
                double& contrast);

        static const int DEFAULT_WHITE_TEMPERATURE;


    private:
        virtual void updateKernel(double variance, int size);
        static void buildLowpassKernel(float kernel[], double variance, int size);
        static void updateLowpassKernelUniform(cellar::GlProgram& prog, float kernel[], int size);


    private:
        unsigned int _colorBufferTexId;
        unsigned int _depthBufferTexId;

        cellar::GlProgram _postProdProgram;
        unsigned int _fullscreenVao;
        unsigned int _fullscreenVbo;
        bool _subroutineAvailable;

        bool _lowpassActivated;
        unsigned int _filteringFunc;
        float _lowpassKernel[25];
        double _lowpassKernelVar;
        int _lowpassKernelSize;
        bool _adaptationActivated;
        float _adaptationFactor;
        glm::vec3 _temperatureColor;
        float _luminosityValue;
        float _contrastValue;

        bool _isSetup;
    };
}

#endif // PROPROOM3D_GLPOSTPRODUNIT_H
