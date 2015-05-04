#include "CpuRaytracerWorker.h"

#include <GLM/gtc/random.hpp>

#include "../../Prop/Prop.h"
#include "../../Prop/Volume/Volume.h"
#include "../../Prop/Volume/Raycast.h"
#include "../../Prop/Costume/Costume.h"


namespace prop3
{
    void CpuRaytracerWorker::launchWorker(
        const std::shared_ptr<CpuRaytracerWorker>& worker)
    {
        worker->execute();
    }

    CpuRaytracerWorker::CpuRaytracerWorker() :
        _isSingleShot(false),
        _runningPredicate(false),
        _terminatePredicate(false),
        _usePixelJittering(true),
        _useStochasticTracing(true),
        _lightRaysBounceCount(0),
        _screenRaysBounceCount(5),
        _viewportSize(1, 1),
        _workingColorBuffer(nullptr)
    {
        int bufferSize = _viewportSize.x * _viewportSize.y * 3;
        _workingColorBuffer = new float[bufferSize];
    }

    CpuRaytracerWorker::~CpuRaytracerWorker()
    {
        destroyBuffers();
    }

    void CpuRaytracerWorker::start(bool singleShot)
    {
        _isSingleShot = singleShot;
        if(!_runningPredicate)
        {
            _runningPredicate = true;
            _cv.notify_one();
        }
    }

    void CpuRaytracerWorker::stop()
    {
        if(_runningPredicate)
        {
            _runningPredicate = false;
        }
    }

    void CpuRaytracerWorker::terminate()
    {
        _terminatePredicate = true;

        if(_runningPredicate)
        {
            // Skip current frame
            _runningPredicate = false;
            std::lock_guard<std::mutex> lk(_flowMutex);
        }

        _cv.notify_one();
    }

    bool CpuRaytracerWorker::isRunning()
    {
        return _runningPredicate;
    }

    void CpuRaytracerWorker::updateView(const glm::dmat4& view)
    {
        skipAndExecute([this, &view](){
            _viewInvMatrix = glm::inverse(view);
            _viewProjInverse = _viewInvMatrix * _projInvMatrix;
            _camPos = glm::dvec3(_viewInvMatrix * glm::dvec4(0, 0, 0, 1));
        });
    }

    void CpuRaytracerWorker::updateProjection(const glm::dmat4& proj)
    {
        skipAndExecute([this, &proj](){
            _projInvMatrix = glm::inverse(proj);
            _viewProjInverse = _viewInvMatrix * _projInvMatrix;
        });
    }

    void CpuRaytracerWorker::updateViewport(
            const glm::ivec2& resolution,
            const glm::ivec2& origin,
            const glm::ivec2& size)
    {
            skipAndExecute([this, &resolution, &origin, &size](){
                _viewportOrig = origin;
                if(size != _viewportSize)
                {
                    _resolution = resolution;
                    _viewportSize = size;
                    destroyBuffers();
                    getNewWorkingBuffers();
                }
            });
    }

    void CpuRaytracerWorker::setProps(const std::vector<std::shared_ptr<Prop>>& props)
    {
        skipAndExecute([this, &props](){
            _props = props;
        });
    }

    void CpuRaytracerWorker::useStochasticTracing(bool use)
    {
        _useStochasticTracing = use;
    }

    void CpuRaytracerWorker::usePixelJittering(bool use)
    {
        _usePixelJittering = use;
    }

    unsigned int CpuRaytracerWorker::completedFrameCount()
    {
        std::lock_guard<std::mutex> lk(_framesMutex);
        return _completedColorBuffers.size();
    }

    const float* CpuRaytracerWorker::readNextFrame()
    {
        _framesMutex.lock();
        return _completedColorBuffers.front();
    }

    void CpuRaytracerWorker::popReadFrame()
    {
        _framePool.push_back(_completedColorBuffers.front());
        _completedColorBuffers.pop();
        _framesMutex.unlock();
    }

    void CpuRaytracerWorker::skipAndExecute(const std::function<void()>& func)
    {
        // Skip current frame
        bool isRunning = _runningPredicate;
        if(isRunning)
        {
            _runningPredicate = false;
        }

        // Lock and execute
        std::lock_guard<std::mutex> lk(_flowMutex);
        resetBuffers();
        func();

        // Begin next frame
        if(isRunning)
        {
            _runningPredicate = true;
            _cv.notify_one();
        }
    }

    void CpuRaytracerWorker::execute()
    {
        while(true)
        {
            std::unique_lock<std::mutex> lk(_flowMutex);
            _cv.wait(lk, [this]{
                return (_runningPredicate && !_props.empty()) ||
                        _terminatePredicate;
            });

            // Verify if we are supposed to terminate
            if(_terminatePredicate)
            {
                return;
            }


            // Shoot rays
            if(_lightRaysBounceCount != 0)
                shootFromLights();

            if(_screenRaysBounceCount != 0)
                shootFromScreen();


            // Verify that stop or skip was not called
            if(_runningPredicate)
            {
                commitWorkingBuffers();

                if(_isSingleShot)
                {
                    _runningPredicate = false;
                }
            }
        }
    }

    void CpuRaytracerWorker::shootFromLights()
    {
    }

    void CpuRaytracerWorker::shootFromScreen()
    {
        double pixelWidth = 2.0 / _resolution.x;
        double pixelHeight = 2.0 / _resolution.y;
        glm::dvec2 pixelSize(pixelWidth, pixelHeight);
        glm::dvec2 imgOrig = -glm::dvec2(_resolution) / 2.0;
        glm::dvec2 orig =  (imgOrig + glm::dvec2(_viewportOrig)) * pixelSize;

        if(_usePixelJittering)
        {
            orig += (glm::dvec2(0.5) + glm::circularRand(0.5)) * pixelSize;
        }

        int idx = -1;
        Ray ray(_camPos, glm::dvec3(0));
        glm::dvec4 screenPos(-1, -1, 0, 1);

        screenPos.y = orig.y;
        for(int j=0; j<_viewportSize.y; ++j, screenPos.y += pixelHeight)
        {
            screenPos.x = orig.x;
            for(int i=0; i<_viewportSize.x; ++i, screenPos.x += pixelWidth)
            {
                glm::dvec4 dirH = _viewProjInverse * screenPos;
                ray.direction = glm::normalize(glm::dvec3(dirH / dirH.w) - _camPos);

                glm::dvec3 color = fireScreenRay(ray, _screenRaysBounceCount);

                _workingColorBuffer[++idx] = color.r;
                _workingColorBuffer[++idx] = color.g;
                _workingColorBuffer[++idx] = color.b;
            }

            // Verify if this frame must be skipped
            // This verification is only done by line
            if(!_runningPredicate)
                return;
        }
    }

    std::shared_ptr<Prop> CpuRaytracerWorker::findNearestProp(
            const Ray& ray, RaycastReport& reportMin)
    {
        std::shared_ptr<Prop> propMin;


        for(const auto& prop : _props)
        {
            const std::shared_ptr<Volume>& volume = prop->volume();

            if(volume.get() == nullptr)
                continue;

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

        return propMin;
    }

    glm::dvec3 CpuRaytracerWorker::fireScreenRay(const Ray& ray, int depth)
    {
        if(depth < 0)
        {
            return glm::dvec3(0);
        }

        RaycastReport reportMin(INFINITY, glm::dvec3(), glm::dvec3());
        std::shared_ptr<Prop> propMin = findNearestProp(ray, reportMin);
        glm::dvec3 color(0);

        if(propMin)
        {
            // Surface color
            const std::shared_ptr<Costume>& costume = propMin->costume();

            if(!_useStochasticTracing)
            {
                return propMin->costume()->computeReflectionBrdf(
                            -glm::reflect(ray.direction, reportMin.normal),
                            reportMin.normal,
                            -ray.direction,
                            reportMin.texCoord);
            }

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

    void CpuRaytracerWorker::resetBuffers()
    {
        while(!_completedColorBuffers.empty())
        {
            _framePool.push_back(_completedColorBuffers.front());
            _completedColorBuffers.pop();
        }
    }

    void CpuRaytracerWorker::destroyBuffers()
    {
        if(_workingColorBuffer != nullptr)
        {
            delete[] _workingColorBuffer;
            _workingColorBuffer = nullptr;
        }

        while(!_completedColorBuffers.empty())
        {
            delete[] _completedColorBuffers.front();
            _completedColorBuffers.pop();
        }

        for(float* frame : _framePool)
        {
            delete[] frame;
        }
        _framePool.clear();
    }

    void CpuRaytracerWorker::getNewWorkingBuffers()
    {
        if(_framePool.empty())
        {
            int bufferSize = _viewportSize.x * _viewportSize.y * 3;
            _workingColorBuffer = new float[bufferSize];
        }
        else
        {
            _workingColorBuffer = _framePool.back();
            _framePool.pop_back();
        }
    }

    void CpuRaytracerWorker::commitWorkingBuffers()
    {
        _framesMutex.lock();

        _completedColorBuffers.push(
            _workingColorBuffer);
        getNewWorkingBuffers();

        _framesMutex.unlock();
    }
}
