#include "QGlPostProdUnit.h"
#include "ui_QGlPostProdUnit.h"

#include <cassert>

#include <GLM/gtc/constants.hpp>

#include <QFileDialog>

#include <CellarWorkbench/Misc/CellarUtils.h>
using namespace cellar;


namespace prop3
{
    const int QGlPostProdUnit::DEFAULT_WHITE_TEMPERATURE = 6500;

    QGlPostProdUnit::QGlPostProdUnit() :
        _ui(new Ui::QGlPostProdUnit()),
        _colorBufferTexId(0),
        _fullscreenVao(0),
        _fullscreenVbo(0)
    {
        _ui->setupUi(this);
    }

    QGlPostProdUnit::~QGlPostProdUnit()
    {
        glDeleteVertexArrays(1, &_fullscreenVao);
        glDeleteBuffers(1, &_fullscreenVbo);

        _fullscreenVao = 0;
        _fullscreenVbo = 0;
    }

    void QGlPostProdUnit::setColorBufferTexId(unsigned int id)
    {
        _colorBufferTexId = id;
    }

    void QGlPostProdUnit::setup()
    {
        // Post production program
        _postProdProgram.addShader(GL_VERTEX_SHADER, ":/Prop3/shaders/clip_space.vert");
        _postProdProgram.addShader(GL_FRAGMENT_SHADER, ":/Prop3/shaders/post_prod.frag");
        _postProdProgram.link();
        _postProdProgram.pushProgram();
        _postProdProgram.setInt("ImageTex", 0);
        _postProdProgram.setFloat("AdaptationFactor", _adaptationFactor);
        _postProdProgram.setFloat("LuminosityValue", _luminosityValue);
        _postProdProgram.setFloat("ContrastValue", _contrastValue);
        _postProdProgram.setVec3f("TemperatureRgb", cellar::Vec3f(
            _temperatureColor.r,
            _temperatureColor.g,
            _temperatureColor.b));
        _postProdProgram.popProgram();


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

        // Setup signal and slots

        lowpassSizeChanged(_ui->lowpassSizeCombo->currentIndex());
        connect(_ui->lowpassSizeCombo, (void (QComboBox::*)(int)) &QComboBox::currentIndexChanged,
                this,                  &QGlPostProdUnit::lowpassSizeChanged);

        lowpassVarianceChanged(_ui->lowpassVarianceSpin->value());
        connect(_ui->lowpassVarianceSpin, (void (QDoubleSpinBox::*)(double)) &QDoubleSpinBox::valueChanged,
                this,                     &QGlPostProdUnit::lowpassVarianceChanged);

        adaptativeFilteringFactorChanged(_ui->adaptativeFactorSlider->value());
        connect(_ui->adaptativeFactorSlider, &QSlider::valueChanged,
                this,                        &QGlPostProdUnit::adaptativeFilteringFactorChanged);

        useAdaptativeFilteringChecked(_ui->useAdaptativeFilteringCheck->checkState());
        connect(_ui->useAdaptativeFilteringCheck, &QCheckBox::stateChanged,
                this,                             &QGlPostProdUnit::useAdaptativeFilteringChecked);

        activateLowPassChecked(_ui->activateLowpassCheck->checkState());
        connect(_ui->activateLowpassCheck, &QCheckBox::stateChanged,
                this,                      &QGlPostProdUnit::activateLowPassChecked);

        temperatureChanged(_ui->temperatureSpin->value());
        connect(_ui->temperatureSpin,   (void (QSpinBox::*)(int)) &QSpinBox::valueChanged,
                this,                   &QGlPostProdUnit::temperatureChanged);

        temperatureDefaultClicked();
        connect(_ui->temperatureDefaultBtn, &QPushButton::clicked,
                this,                       &QGlPostProdUnit::temperatureDefaultClicked);

        contrastChanged(_ui->contrastSlider->value());
        connect(_ui->contrastSlider, &QSlider::valueChanged,
                this,                &QGlPostProdUnit::contrastChanged);

        luminosityChanged(_ui->luminositySlider->value());
        connect(_ui->luminositySlider, &QSlider::valueChanged,
                this,                  &QGlPostProdUnit::luminosityChanged);

        connect(_ui->saveButton, &QPushButton::clicked,
                this,            &QGlPostProdUnit::saveOutputImage);


        // Show up the panel
        show();
    }

    void QGlPostProdUnit::execute()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);

        _postProdProgram.pushProgram();

        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &_filteringFunc);

        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(_fullscreenVao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);

        _postProdProgram.popProgram();
    }

    void QGlPostProdUnit::lowpassSizeChanged(int sizeIndex)
    {
        if(sizeIndex == 0) _lowpassKernelSize = 3;
        else if(sizeIndex == 1) _lowpassKernelSize = 5;
        else assert(false /* Unsupported kernel size */);

        updateKernel(_lowpassKernelVar, _lowpassKernelSize);
    }

    void QGlPostProdUnit::activateLowPassChecked(int state)
    {
        bool isChecked = state;
        _ui->lowpassWidget->setEnabled(isChecked);

        if(isChecked)
        {
            _filteringFunc = _ui->adaptativeWidget->isEnabled() ? 2 : 1;
        }
        else
        {
            _filteringFunc = 0;
        }
    }

    void QGlPostProdUnit::lowpassVarianceChanged(double variance)
    {
        if(variance < 0.001) variance = 0.0;
        _lowpassKernelVar = variance;

        updateKernel(_lowpassKernelVar, _lowpassKernelSize);
    }

    void QGlPostProdUnit::useAdaptativeFilteringChecked(int state)
    {
        bool isChecked = state;
        _ui->adaptativeWidget->setEnabled(isChecked);\

        if(isChecked)
        {
            _filteringFunc = 2;
        }
        else
        {
            _filteringFunc = 1;
        }
    }

    void QGlPostProdUnit::adaptativeFilteringFactorChanged(int factor)
    {
        _adaptationFactor = computeAdaptativeFactor(factor);

        _postProdProgram.pushProgram();
        _postProdProgram.setFloat("AdaptationFactor", _adaptationFactor);
        _postProdProgram.popProgram();

        _ui->adaptativeFactorLabel->setText(QString::number(factor) + '%');
    }

    void QGlPostProdUnit::temperatureChanged(int kelvin)
    {
        _temperatureColor = kelvinToRgb(kelvin);

        _postProdProgram.pushProgram();
        _postProdProgram.setVec3f("TemperatureRgb", cellar::Vec3f(
            _temperatureColor.r,
            _temperatureColor.g,
            _temperatureColor.b));
        _postProdProgram.popProgram();


        _ui->temperatureDefaultBtn->setEnabled(
            kelvin != DEFAULT_WHITE_TEMPERATURE);
    }

    void QGlPostProdUnit::temperatureDefaultClicked()
    {
        _ui->temperatureSpin->setValue(DEFAULT_WHITE_TEMPERATURE);
    }

    void QGlPostProdUnit::contrastChanged(int contrast)
    {
        _contrastValue = computeContrastFactor(contrast);
        _ui->contrastLabel->setText(
            QString::number((int)(_contrastValue*100)) + '%');

        _postProdProgram.pushProgram();
        _postProdProgram.setFloat("ContrastValue", _contrastValue);
        _postProdProgram.popProgram();
    }

    void QGlPostProdUnit::luminosityChanged(int luminosity)
    {
        _luminosityValue = computeLuminosityFactor(luminosity);
        _ui->luminosityLabel->setText(
            QString::number((double)_luminosityValue, 'g', 2));

        _postProdProgram.pushProgram();
        _postProdProgram.setFloat("LuminosityValue", _luminosityValue);
        _postProdProgram.popProgram();
    }

    void QGlPostProdUnit::saveOutputImage()
    {
        QFileDialog saveDialog(this);
        saveDialog.setAcceptMode(QFileDialog::AcceptSave);
        saveDialog.setNameFilter("Images (*.png *.jpg)");
        saveDialog.setDefaultSuffix("png");

        if(saveDialog.exec())
        {
            QString fileName = saveDialog.selectedFiles().at(0);

            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);
            int width = viewport[2];
            int height = viewport[3];

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            GLubyte* frags = new GLubyte[width * height * 4];
            glReadPixels(viewport[0], viewport[1], viewport[2], viewport[3],
                         GL_RGBA, GL_UNSIGNED_BYTE, frags);

            int pIdx = -1;
            int fIdx = (height-1) * width * 4;
            uchar* pixels = new uchar[width * height * 4];
            for(int j=0; j<height; ++j)
            {
                for(int i=0; i<width; ++i)
                {
                    pixels[++pIdx] = frags[fIdx+2]; // Blue
                    pixels[++pIdx] = frags[fIdx+1]; // Green
                    pixels[++pIdx] = frags[fIdx+0]; // Red
                    pixels[++pIdx] = 255; // Alpha
                    fIdx += 4;
                }

                fIdx -= 2*width * 4;
            }

            QImage image(pixels, width, height, QImage::Format_RGB32);
            image.save(fileName);

            delete frags;
            delete pixels;
        }
    }

    void QGlPostProdUnit::updateKernel(double variance, int size)
    {
        buildLowpassKernel(_lowpassKernel, variance, size);
        updateLowpassKernelTable(_ui->kernelTable, _lowpassKernel);
        updateLowpassKernelUniform(_postProdProgram, _lowpassKernel, size);
    }

    glm::vec3 QGlPostProdUnit::kelvinToRgb(int kelvin)
    {
        glm::vec3 color;
        double scaledKelvin = kelvin / 100.0;

        if(kelvin < 6600)
        {
            // Red
            color.r = 1.0;

            // Green
            double tmpG = scaledKelvin;
            tmpG = 0.390081579 * log(tmpG) - 0.631841444;
            color.g = glm::clamp(tmpG, 0.0, 1.0);

            // Blue
            double tmpB = scaledKelvin - 10;
            tmpB = 0.543206789 * log(tmpB) - 1.196254089;
            color.b = glm::clamp(tmpB, 0.0, 1.0);
        }
        else
        {
            // Red
            double tpmR = scaledKelvin - 60.0;
            tpmR = 1.292936186 * pow(tpmR, -0.1332047592);
            color.r = glm::clamp(tpmR, 0.0, 1.0);

            // Green
            double tmpG = scaledKelvin - 60.0;
            tmpG = 1.129890861 * pow(tmpG, -0.0755148492);
            color.g = glm::clamp(tmpG, 0.0, 1.0);

            // Blue
            color.b = 1.0;
        }

        return color;
    }

    float QGlPostProdUnit::computeLuminosityFactor(int luminosity)
    {
        return (luminosity - 50) / 50.0f;
    }

    float QGlPostProdUnit::computeContrastFactor(int contrast)
    {
        return contrast / 50.0f;
    }

    float QGlPostProdUnit::computeAdaptativeFactor(int factor)
    {
        return factor / 100.0f;
    }

    void QGlPostProdUnit::buildLowpassKernel(float kernel[], double variance, int size)
    {
        int halfSize = size /2;
        float mass = 0.0;

        for(int j=-2; j<3; ++j)
        {
            for(int i=-2; i<3; ++i)
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

    void QGlPostProdUnit::updateLowpassKernelTable(QTableWidget* widget, float kernel[])
    {
        for(int j=0; j<5; ++j)
        {
            for(int i=0; i<5; ++i)
            {
                QString cellValue;
                float value = kernel[j*5 + i];
                if(value != 0) cellValue = QString::number(value, 'g', 2);
                widget->setCellWidget(i, j, new QLabel(cellValue));
            }
        }
    }

    void QGlPostProdUnit::updateLowpassKernelUniform(media::GlProgram& prog, float kernel[], int size)
    {
        prog.pushProgram();
        prog.setInt("LowpassSize", (size / 2));
        for(int i=0; i<25; ++i)
        {
            std::string name = "LowpassKernel[" + toString(i) + "]";
            prog.setFloat(name, kernel[i]);
        }
        prog.popProgram();
    }
}
