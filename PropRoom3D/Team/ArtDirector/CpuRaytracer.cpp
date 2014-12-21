#include "CpuRaytracer.h"

#include <algorithm>

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/random.hpp>

#include <GL3/gl3w.h>

#include <CellarWorkbench/Misc/CellarUtils.h>
#include <CellarWorkbench/Misc/Log.h>

#include <MediaWorkbench/Camera/Camera.h>

#include "../../Prop/Prop.h"
#include "../../Prop/Costume/Costume.h"
#include "../../Prop/Volume/Raycast.h"


namespace prop3
{
    /* Too hard for the moment
    class CpuRaytracerWorker
    {
    public:
        CpuRaytracerWorker(const std::shared_ptr<CpuRaytracer>& tracer) :
            _tracer(tracer)
        {
        }

        virtual void execute();

    protected:
        virtual void resizeBuffers();
        virtual void clearBuffers();
        virtual void shootFromLights();
        virtual void shootFromScreen();
        virtual glm::dvec3 fireScreenRay(const Ray& ray, int depth);

    private:
        std::shared_ptr<CpuRaytracer> _tracer;
    };

    void launchWorker(const std::shared_ptr<CpuRaytracer>& tracer)
    {
        CpuRaytracerWorker worker(tracer);
        worker.execute();
    }
    */


    CpuRaytracer::CpuRaytracer() :
        _sceneChanged(true),
        _lightRaysBounceCount(0),
        _screenRaysBounceCount(5),
        _viewportSize(1, 1),
        _sampleCount(0),
        _colorBuffer(1)
    {
    }

    CpuRaytracer::~CpuRaytracer()
    {

    }

    void CpuRaytracer::setup()
    {
        glm::dvec3 upVec(0, 0, 1);
        glm::dvec3 focusPos = glm::dvec3(-1.2, -1.2, 5.25);
        _camPos = focusPos + glm::dvec3(25, -40, 14) * 2.0;
        _viewMatrix = glm::lookAt(_camPos, focusPos, upVec);


        glGenTextures(1, &_colorBufferGlId);
        glBindTexture(GL_TEXTURE_2D, _colorBufferGlId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_DOUBLE, nullptr);

        glGenFramebuffers(1, &_framebufferGlId);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebufferGlId);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, _colorBufferGlId, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void CpuRaytracer::reset()
    {
        _props.clear();
    }

    void CpuRaytracer::draw(double dt)
    {
        if(_sceneChanged)
        {
            clearBuffers();
            _sceneChanged = false;
        }

        if(_lightRaysBounceCount != 0)
            shootFromLights();

        shootFromScreen();
        sendBuffersToGpu();
    }

    void CpuRaytracer::notify(media::CameraMsg &msg)
    {
        _sceneChanged = true;

        if(msg.change == media::CameraMsg::EChange::PROJECTION)
        {
            _viewportSize.x = msg.camera.viewport().x();
            _viewportSize.y = msg.camera.viewport().y();
            resizeBuffers();
        }
    }

    void CpuRaytracer::manageProp(const std::shared_ptr<Prop>& prop)
    {
        _props.push_back(prop);
    }

    void CpuRaytracer::unmanageProp(const std::shared_ptr<Prop>& prop)
    {
        std::remove_if(_props.begin(), _props.end(),
            [&prop](const std::shared_ptr<Prop>& p) {
                return p == prop;
        });
    }

    void CpuRaytracer::resizeBuffers()
    {
        glm::dmat4 proj = glm::perspectiveFov(
            glm::pi<double>() / 9.0,
            (double) _viewportSize.x,
            (double) _viewportSize.y,
            1.0, 2.0);

        _viewProjInverse = glm::inverse(proj * _viewMatrix);

        _colorBuffer.resize(_viewportSize.x * _viewportSize.y * 3);
        clearBuffers();
    }

    void CpuRaytracer::clearBuffers()
    {
        int idx = -1;
        for(int j=0; j<_viewportSize.y; ++j)
        {
            for(int i=0; i<_viewportSize.x; ++i)
            {
                _colorBuffer[++idx] = 0.0;
                _colorBuffer[++idx] = 0.0;
                _colorBuffer[++idx] = 0.0;
            }
        }

        _sampleCount = 0;
    }

    void CpuRaytracer::shootFromLights()
    {

    }

    void CpuRaytracer::shootFromScreen()
    {
        double pixelWidth = 2.0 / _viewportSize.x;
        double pixelHeight = 2.0 / _viewportSize.y;
        glm::dvec4 screenPos(-1, -1, 0, 1);
        glm::dvec2 offset = glm::dvec2(pixelWidth, pixelWidth) *
                            (glm::dvec2(0.5) + glm::circularRand(0.5));

        int idx = 0;
        ++_sampleCount;
        double mix = 1.0 / (_sampleCount);
        Ray ray(_camPos, glm::dvec3(0));

        screenPos.y = -1.0 + offset.y;
        for(int j=0; j<_viewportSize.y; ++j, screenPos.y += pixelHeight)
        {
            screenPos.x = -1.0 + offset.x;
            for(int i=0; i<_viewportSize.x; ++i, screenPos.x += pixelWidth)
            {
                glm::dvec4 dirH = _viewProjInverse * screenPos;
                ray.direction = glm::normalize(glm::dvec3(dirH / dirH.w) - _camPos);

                glm::dvec3 color = fireScreenRay(ray, _screenRaysBounceCount);

                _colorBuffer[idx] = glm::mix((double)_colorBuffer[idx], color.r, mix);
                ++idx;
                _colorBuffer[idx] = glm::mix((double)_colorBuffer[idx], color.g, mix);
                ++idx;
                _colorBuffer[idx] = glm::mix((double)_colorBuffer[idx], color.b, mix);
                ++idx;
            }
        }
    }

    void CpuRaytracer::sendBuffersToGpu()
    {
        glBindTexture(GL_TEXTURE_2D, _colorBufferGlId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     _viewportSize.x, _viewportSize.y,
                     0, GL_RGB, GL_FLOAT, _colorBuffer.data());

        glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebufferGlId);
        glReadBuffer(GL_COLOR_ATTACHMENT0);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, _viewportSize.x, _viewportSize.y,
                          0, 0, _viewportSize.x, _viewportSize.y,
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    glm::dvec3 CpuRaytracer::fireScreenRay(const Ray& ray, int depth)
    {
        if(depth < 0)
        {
            return glm::dvec3(0);
        }

        RaycastReport reportMin(INFINITY, glm::dvec3(), glm::dvec3());
        std::shared_ptr<Prop> propMin;
        glm::dvec3 color(0);


        for(const auto& prop : _props)
        {
            const std::shared_ptr<Volume>& volume = prop->volume();
            std::vector<RaycastReport> reports;
            volume->raycast(ray, reports);

            for(const RaycastReport& report : reports)
            {
                if(0.0 < report.t && report.t < reportMin.t )
                {
                    reportMin = report;
                    propMin = prop;
                }
            }
        }

        if(propMin)
        {
            // Surface color
            const std::shared_ptr<Costume>& costume = propMin->costume();

            const glm::dvec3& interPt = reportMin.position;
            bool isEntering = glm::dot(ray.direction, reportMin.normal) < 0.0;
            double offSetDist = (0.000001 * (isEntering ? 1.0 : -1.0));
            glm::dvec3 offset = reportMin.normal * offSetDist;

            double reflectionRation = costume->computeReflexionRatio(
                                          ray.direction,
                                          reportMin.normal,
                                          reportMin.texCoord);

            // Reflection
            glm::dvec3 reflect = costume->computeReflection(
                                     ray.direction,
                                     reportMin.normal,
                                     reportMin.texCoord);

            Ray reflectedRay(interPt + offset, reflect);
            color += fireScreenRay(reflectedRay, depth-1) *
                     reflectionRation;

            // Refraction
            if(reflectionRation != 1.0)
            {
                glm::dvec3 refract = costume->computeRefraction(
                                         ray.direction,
                                         reportMin.normal,
                                         reportMin.texCoord);

                Ray refractedRay(interPt - offset, refract);
                color += fireScreenRay(refractedRay, depth-1) *
                         (1.0 - reflectionRation);
            }

            color *= propMin->costume()->computeReflectionBrdf(
                        -reflectedRay.direction,
                        reportMin.normal,
                        -ray.direction,
                         reportMin.texCoord);
        }
        else if(depth != _screenRaysBounceCount)
        {
            // Background color
            const glm::dvec3 blueSkyColor = glm::dvec3(0.25, 0.60, 1.00) * 2.0;
            const glm::dvec3 skylineColor = glm::dvec3(1.00, 1.00, 1.00) * 2.0;
            const glm::dvec3 sunColor = glm::dvec3(1.00, 0.7725, 0.5608) * 20.0;

            const glm::dvec3 blueSkyDir = glm::dvec3(0, 0, 1);
            const glm::dvec3 sunDir = glm::dvec3(0.4082, 0.4082, 0.8165);

            double upDot = glm::dot(blueSkyDir, ray.direction);
            glm::dvec3 blueSky = blueSkyColor * glm::max(0.0, upDot);

            double a = 1.0 - glm::abs(upDot);
            a *= a *= a *= a *= a;
            glm::dvec3 skyline = skylineColor * a;

            double s = glm::max(0.0, glm::dot(sunDir, ray.direction));
            s *= s *= s *= s *= s *= s;
            glm::dvec3 sun = sunColor * s;

            color = blueSky + skyline + sun;
        }

        return color;
    }
}
