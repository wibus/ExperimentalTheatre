#include "GlPostProdUnit.h"

#include <cassert>

#include <GLM/gtc/constants.hpp>

#include <QFileDialog>

#include <CellarWorkbench/Image/Image.h>
#include <CellarWorkbench/GL/GlToolkit.h>
#include <CellarWorkbench/Misc/StringUtils.h>
#include <CellarWorkbench/Misc/Log.h>

#include <CellarWorkbench/GL/GlInputsOutputs.h>

#include "Node/Light/LightUtils.h"


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
        _exposureGain(1.0, 1.0, 1.0),
        _isAcesTonemappingActive(true),
        _temperatureColor(1.0),
        _contrastValue(1.0f),
        _gammaValue(2.0f),
        _middleGrayValue(0.5f),
        _isSetup(false)
    {
    }

    GlPostProdUnit::~GlPostProdUnit()
    {
        glDeleteTextures(1, &_colorBufferTexId);
        glDeleteVertexArrays(1, &_fullscreenVao);
        glDeleteBuffers(1, &_fullscreenVbo);

        _colorBufferTexId = 0;
        _fullscreenVao = 0;
        _fullscreenVbo = 0;
    }

    const float* GlPostProdUnit::lowpassKernel() const
    {
        return &(_lowpassKernel[0]);
    }

    void GlPostProdUnit::updateDepthRange(const glm::vec2& range)
    {
        _postProdProgram.pushProgram();
        _postProdProgram.setVec2f("DepthRange", range);
        _postProdProgram.popProgram();
    }

    void GlPostProdUnit::setup()
    {
        // Full screen triangle VAO
        const float vertices [][3] =  {
            {-1, -1, 1},
            { 3, -1, 1},
            {-1,  3, 1}
        };


        // Quad VAO
        glGenVertexArrays(1, &_fullscreenVao);
        glBindVertexArray(_fullscreenVao);

        glGenBuffers(1, &_fullscreenVbo);
        glBindBuffer(GL_ARRAY_BUFFER, _fullscreenVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        // Color texture
        glGenTextures(1, &_colorBufferTexId);
        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Depth texture
        glGenTextures(1, &_depthBufferTexId);
        glBindTexture(GL_TEXTURE_2D, _depthBufferTexId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        clearOutput();


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
        _postProdProgram.setInt("DepthTex", 1);
        _postProdProgram.setVec2f("DepthRange", glm::vec2(0, 1));
        _postProdProgram.setFloat("AdaptationFactor", _adaptationFactor);
        _postProdProgram.setVec3f("ExposureGain", _exposureGain);
        _postProdProgram.setInt("AcesTonemappingActive", _isAcesTonemappingActive ? 1 : 0);
        _postProdProgram.setFloat("MiddleGrayValue",  _middleGrayValue);
        _postProdProgram.setFloat("ContrastValue",    _contrastValue);
        _postProdProgram.setFloat("GammaValue",       _gammaValue);
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

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _depthBufferTexId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);

        if(_subroutineAvailable)
        {
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &_filteringFunc);
        }

        glBindVertexArray(_fullscreenVao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        _postProdProgram.popProgram();
    }

    void GlPostProdUnit::clearOutput()
    {
        const float maxDepth = INFINITY;
        const float black[] = {0, 0, 0};

        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, black);

        glBindTexture(GL_TEXTURE_2D, _depthBufferTexId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 1, 1, 0,
                     GL_RED, GL_FLOAT, &maxDepth);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GlPostProdUnit::update(Film& film,
            Film::ColorOutput& colorOutput)
    {
        glm::ivec2 viewportSize = film.frameResolution();
        const std::vector<float>& depthBuffer = film.depthBuffer();
        const std::vector<glm::vec3>& colorBuffer = film.colorBuffer(colorOutput);

        // Send image to GPU
        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);
        glTexImage2D(GL_TEXTURE_2D,         0,  GL_RGB32F,
                     viewportSize.x,        viewportSize.y,
                     0, GL_RGB, GL_FLOAT,   colorBuffer.data());

        if(!depthBuffer.empty())
        {
            glBindTexture(GL_TEXTURE_2D, _depthBufferTexId);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F,
                         viewportSize.x,        viewportSize.y,
                         0, GL_RED, GL_FLOAT, depthBuffer.data());
        }

        glBindTexture(GL_TEXTURE_2D, 0);
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

    void GlPostProdUnit::setImageContrast(float contrast)
    {
        _contrastValue = contrast;

        if(_isSetup)
        {
            _postProdProgram.pushProgram();
            _postProdProgram.setFloat("ContrastValue", _contrastValue);
            _postProdProgram.popProgram();
        }
    }

    void GlPostProdUnit::setImageMiddleGray(float middleGray)
    {
        _middleGrayValue = middleGray;

        if(_isSetup)
        {
            _postProdProgram.pushProgram();
            _postProdProgram.setFloat("MiddleGrayValue", _middleGrayValue);
            _postProdProgram.popProgram();
        }
    }

    void GlPostProdUnit::setAcesTonemappingActive(bool isActive)
    {
        _isAcesTonemappingActive = isActive;

        if(_isSetup)
        {
            _postProdProgram.pushProgram();
            _postProdProgram.setInt("AcesTonemappingActive", _isAcesTonemappingActive ? 1 : 0);
            _postProdProgram.popProgram();
        }
    }

    void GlPostProdUnit::setExposureGain(const glm::vec3& exposure)
    {
        _exposureGain = exposure;

        if(_isSetup)
        {
            _postProdProgram.pushProgram();
            _postProdProgram.setVec3f("ExposureGain", _exposureGain);
            _postProdProgram.popProgram();
        }
    }

    void GlPostProdUnit::setImageGamma(float gamma)
    {
        _gammaValue = gamma;

        if(_isSetup)
        {
            _postProdProgram.pushProgram();
            _postProdProgram.setFloat("GammaValue", _gammaValue);
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

    double GlPostProdUnit::getAutoExposure(double targetAverageLuminance)
    {
        if(_colorBufferTexId != 0)
        {
            GLint width, height;
            glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

            GLfloat* buffer = new GLfloat[width*height * 3];
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, buffer);
            glBindTexture(GL_TEXTURE_2D, 0);

            size_t idx = 0;
            double avgLogLum = 0.0;
            double weightSum = 0.0;
            double var = width * height / 8.0;
            for(int h=0, dh=-height/2; h < height; ++h, ++dh)
            {
                for(int w=0, dw=-width/2; w < width; ++w, ++dw)
                {
                    double weight = glm::exp(-(dw*dw + dh*dh) / var);
                    glm::dvec3 pixel(buffer[idx], buffer[idx+1], buffer[idx+2]);
                    avgLogLum += glm::log(luminance(pixel) + 1.0e-6) * weight;
                    weightSum += weight;
                    idx += 3;
                }
            }
            avgLogLum = glm::exp(avgLogLum / weightSum);

            delete[] buffer;

            return targetAverageLuminance / glm::sqrt(avgLogLum);
        }
        else
        {
            return 1.0;
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
