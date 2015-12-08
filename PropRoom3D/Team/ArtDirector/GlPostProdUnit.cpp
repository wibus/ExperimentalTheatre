#include "GlPostProdUnit.h"

#include <cassert>

#include <GLM/gtc/constants.hpp>

#include <QFileDialog>

#include <CellarWorkbench/Image/Image.h>
#include <CellarWorkbench/GL/GlToolkit.h>
#include <CellarWorkbench/Misc/StringUtils.h>
#include <CellarWorkbench/Misc/Log.h>

#include <CellarWorkbench/GL/GlInputsOutputs.h>

#include "Light/LightUtils.h"


namespace prop3
{
    const int GlPostProdUnit::DEFAULT_WHITE_TEMPERATURE = 6500;
    const std::string POST_PROD_FRAG_GL130 = ":/Prop3/shaders/post_prod_gl130.frag";
    const std::string POST_PROD_FRAG_GL440 = ":/Prop3/shaders/post_prod_gl440.frag";

    GlPostProdUnit::GlPostProdUnit() :
        _colorBufferTexId(0),
        _fullscreenVao(0),
        _fullscreenVbo(0),
        _subroutineAvailable(false),
        _lowpassActivated(false),
        _filteringFunc(0),
        _lowpassKernelVar(0.0),
        _lowpassKernelSize(3),
        _adaptationActivated(false),
        _adaptationFactor(1.0f),
        _temperatureColor(DEFAULT_WHITE_TEMPERATURE),
        _contrastValue(1.0f),
        _luminosityValue(0.0f),
        _isSetup(false)
    {
    }

    GlPostProdUnit::~GlPostProdUnit()
    {
        glDeleteVertexArrays(1, &_fullscreenVao);
        glDeleteBuffers(1, &_fullscreenVbo);

        _fullscreenVao = 0;
        _fullscreenVbo = 0;
    }

    const float* GlPostProdUnit::lowpassKernel() const
    {
        return &(_lowpassKernel[0]);
    }

    void GlPostProdUnit::setColorBufferTexId(unsigned int id)
    {
        _colorBufferTexId = id;
    }

    void GlPostProdUnit::setup()
    {
        // Full screen triangle VAO
        const float vertices [][3] =  {
            {-1, -1, 1},
            { 3, -1, 1},
            {-1,  3, 1}
        };

        glGenVertexArrays(1, &_fullscreenVao);
        glBindVertexArray(_fullscreenVao);

        glGenBuffers(1, &_fullscreenVbo);
        glBindBuffer(GL_ARRAY_BUFFER, _fullscreenVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        // Compile Post Prod Shader
        cellar::GlInputsOutputs attribs;
        attribs.setInput(0, "position");
        attribs.setOutput(0, "FragColor");

        GLint major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        std::string postProdShaderSrc = "undefined";
        if(major >= 4 && minor >= 4)
        {
            postProdShaderSrc = POST_PROD_FRAG_GL440;
            _subroutineAvailable = true;
        }
        else
        {
            postProdShaderSrc = POST_PROD_FRAG_GL130;
            _subroutineAvailable = false;
        }

        cellar::getLog().postMessage(new cellar::Message('I', false,
            "Choosing " + postProdShaderSrc + " fragment shader for post production.",
            "QGlPostProdUnit"));

        _postProdProgram.setInAndOutLocations(attribs);
        _postProdProgram.addShader(GL_VERTEX_SHADER, ":/Prop3/shaders/clip_space.vert");
        _postProdProgram.addShader(GL_FRAGMENT_SHADER, postProdShaderSrc);
        _postProdProgram.link();
        _postProdProgram.pushProgram();
        _postProdProgram.setInt("ImageTex", 0);
        _postProdProgram.setFloat("AdaptationFactor", _adaptationFactor);
        _postProdProgram.setFloat("ContrastValue",    _contrastValue);
        _postProdProgram.setFloat("LuminosityValue",  _luminosityValue);
        _postProdProgram.setVec3f("TemperatureRgb",   glm::vec3(
            _temperatureColor.r,
            _temperatureColor.g,
            _temperatureColor.b));
        _postProdProgram.popProgram();

        _isSetup = true;
        activateAdaptativeFiltering(_adaptationActivated);
        activateLowPassFilter(_lowpassActivated);
    }

    void GlPostProdUnit::execute()
    {
        glClear(GL_DEPTH_BUFFER_BIT);

        _postProdProgram.pushProgram();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);

        if(_subroutineAvailable)
        {
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &_filteringFunc);
        }

        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(_fullscreenVao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);

        _postProdProgram.popProgram();
    }

    void GlPostProdUnit::activateLowPassFilter(bool activate)
    {
        _lowpassActivated = activate;
        if(_lowpassActivated)
        {
            _filteringFunc = _adaptationActivated ? 2 : 1;
            updateKernel(_lowpassKernelVar, _lowpassKernelSize);
        }
        else
        {
            _filteringFunc = 0;
            updateKernel(0, 1);
        }
    }

    void GlPostProdUnit::activateAdaptativeFiltering(bool enable)
    {
        _adaptationActivated = enable;
        if(_adaptationActivated)
        {
            _filteringFunc = 2;

            if(_isSetup)
            {
                _postProdProgram.pushProgram();
                _postProdProgram.setFloat("AdaptationFactor", _adaptationFactor);
                _postProdProgram.popProgram();
            }
        }
        else
        {
            _filteringFunc = 1;

            if(_isSetup)
            {
                _postProdProgram.pushProgram();
                _postProdProgram.setFloat("AdaptationFactor", 0);
                _postProdProgram.popProgram();
            }
        }
    }

    void GlPostProdUnit::setLowpassKernelSize(KernelSize size)
    {
        if(size == KernelSize::SIZE_3x3) _lowpassKernelSize = 3;
        else if(size == KernelSize::SIZE_5x5) _lowpassKernelSize = 5;
        else assert(false /* Unsupported kernel size */);

        updateKernel(_lowpassKernelVar, _lowpassKernelSize);
    }

    void GlPostProdUnit::setLowpassVariance(double variance)
    {
        if(variance < 0.001) variance = 0.0;
        _lowpassKernelVar = variance;

        updateKernel(_lowpassKernelVar, _lowpassKernelSize);
    }

    void GlPostProdUnit::setAdaptativeFilteringFactor(float zeroToOne)
    {
        _adaptationFactor = zeroToOne;

        if(_isSetup)
        {
            _postProdProgram.pushProgram();
            _postProdProgram.setFloat("AdaptationFactor", _adaptationFactor);
            _postProdProgram.popProgram();
        }
    }

    void GlPostProdUnit::setImageTemperature(int kelvin)
    {
        _temperatureColor = kelvinToRgb(kelvin);

        if(_isSetup)
        {
            _postProdProgram.pushProgram();
            _postProdProgram.setVec3f("TemperatureRgb", _temperatureColor);
            _postProdProgram.popProgram();
        }
    }

    void GlPostProdUnit::setImageContrast(float minusOneToOne)
    {
        _contrastValue = minusOneToOne;

        if(_isSetup)
        {
            _postProdProgram.pushProgram();
            _postProdProgram.setFloat("ContrastValue", _contrastValue);
            _postProdProgram.popProgram();
        }
    }

    void GlPostProdUnit::setImageLuminosity(float zeroToOne)
    {
        _luminosityValue = zeroToOne;

        if(_isSetup)
        {
            _postProdProgram.pushProgram();
            _postProdProgram.setFloat("LuminosityValue", _luminosityValue);
            _postProdProgram.popProgram();
        }
    }

    void GlPostProdUnit::saveOutputImage()
    {
        QFileDialog saveDialog;
        saveDialog.setAcceptMode(QFileDialog::AcceptSave);
        saveDialog.setNameFilter("Images (*.png *.jpg)");
        saveDialog.setDefaultSuffix("png");

        if(saveDialog.exec())
        {
            cellar::Image screenshot;
            cellar::GlToolkit::takeFramebufferShot(screenshot);
            QString fileName = saveDialog.selectedFiles().at(0);
            screenshot.save(fileName.toStdString());
        }
    }

    void GlPostProdUnit::updateKernel(double variance, int size)
    {
        buildLowpassKernel(_lowpassKernel, variance, size);

        if(_isSetup)
        {
            updateLowpassKernelUniform(_postProdProgram, _lowpassKernel, size);
        }
    }

    void GlPostProdUnit::buildLowpassKernel(float kernel[], double variance, int size)
    {
        int halfSize = size /2;
        float mass = 0.0;

        for(int j=-2; j<=2; ++j)
        {
            for(int i=-2; i<=2; ++i)
            {
                int idx = (j+2)*5+i+2;

                if(abs(i) > halfSize || abs(j) > halfSize)
                {
                    kernel[idx] = 0.0;
                }
                else
                {
                    if(variance != 0.0 && size > 1)
                    {
                        kernel[idx] = glm::exp(-(i*i + j*j) / (2.0*variance)) /
                                        (2.0*glm::pi<double>()*variance);
                        mass += kernel[idx];
                    }
                    else
                    {
                        if(i ==0 && j == 0)
                        {
                            kernel[idx] = 1.0;
                            mass += kernel[idx];
                        }
                        else
                        {
                            kernel[idx] = 0.0;
                        }
                    }
                }
            }
        }

        for(int i=0; i<25; ++i)
           kernel[i] = kernel[i] / mass;
    }

    void GlPostProdUnit::updateLowpassKernelUniform(cellar::GlProgram& prog, float kernel[], int size)
    {
        prog.pushProgram();
        prog.setInt("LowpassSize", (size / 2));
        for(int i=0; i<25; ++i)
        {
            std::string name = "LowpassKernel[" + cellar::toString(i) + "]";
            prog.setFloat(name, kernel[i]);
        }
        prog.popProgram();
    }
}
