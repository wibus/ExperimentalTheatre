#include "CpuRaytracerWorker.h"

#include <GLM/gtc/random.hpp>

#include "../../Prop/Prop.h"
#include "../../Prop/Ray/Raycast.h"
#include "../../Prop/Ray/RayHitList.h"
#include "../../Prop/Ray/RayHitReport.h"
#include "../../Prop/Surface/Surface.h"
#include "../../Prop/Coating/Coating.h"
#include "../../Prop/Material/Material.h"
#include "../../Prop/Material/Air.h"

#include "../../Environment/Environment.h"
#include "../../Environment/Backdrop/Backdrop.h"

#include "../../Scene/Scene.h"
#include "../../Scene/SceneJsonReader.h"

#include "../AbstractTeam.h"
#include "../Designer/StdDesigner.h"


namespace prop3
{
    class WorkerTeam : public AbstractTeam
    {
    public:
        WorkerTeam() :
            AbstractTeam(new StdDesigner(), nullptr /* Choreographer */)
        {
        }
    };

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
        _lightRayIntensityThreshold(1.0),
        _screenRayIntensityThreshold(1.0 / 128.0),
        _diffuseRayCount(1),
        _viewportSize(1, 1),
        _workingColorBuffer(nullptr),
        _team(new WorkerTeam())
    {
        int bufferSize = _viewportSize.x * _viewportSize.y * 3;
        _workingColorBuffer = new float[bufferSize];
        _team->setup();
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

    void CpuRaytracerWorker::setSceneStream(const std::string& stream)
    {
        skipAndExecute([this, &stream](){
            _sceneStream = stream;
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
                return (_runningPredicate &&
                            (!_props.empty() ||
                             !_sceneStream.empty())) ||
                        _terminatePredicate;
            });

            // Verify if we are supposed to terminate
            if(_terminatePredicate)
            {
                return;
            }

            // Verify if scene stream was updated
            if(!_sceneStream.empty())
            {
                SceneJsonReader reader;
                reader.deserialize(*_team, _sceneStream);
                _envMaterial = _team->scene()->environment()->ambientMaterial();
                _backdrop = _team->scene()->environment()->backdrop();
                _props = _team->scene()->props();
                _sceneStream.clear();
            }


            // Shoot rays
            if(_lightRayIntensityThreshold != 0)
                shootFromLights();

            if(_screenRayIntensityThreshold != 0)
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


        std::shared_ptr<Material> ambientMaterial =
                findAmbientMaterial(_camPos);
        Raycast raycast(
            Ray(_camPos, glm::dvec3(0)),
            glm::dvec3(1.0),
            ambientMaterial);

        glm::dvec4 screenPos(-1, -1, 0, 1);
        screenPos.y = orig.y;
        int idx = -1;
        for(int j=0; j<_viewportSize.y; ++j, screenPos.y += pixelHeight)
        {
            screenPos.x = orig.x;
            for(int i=0; i<_viewportSize.x; ++i, screenPos.x += pixelWidth)
            {
                glm::dvec4 dirH = _viewProjInverse * screenPos;
                raycast.ray.direction = glm::normalize(glm::dvec3(dirH / dirH.w) - _camPos);


                glm::dvec3 color = fireScreenRay(raycast);

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

    std::shared_ptr<Material> CpuRaytracerWorker::findAmbientMaterial(
            glm::dvec3 position)
    {
        for(const auto& prop : _props)
        {
            const std::shared_ptr<Surface>& surface = prop->surface();

            if(surface.get() == nullptr)
                continue;

            const std::shared_ptr<Surface>& bounds = prop->boundingSurface();
            if(bounds.get() != nullptr && bounds->isIn(position) == EPointPosition::OUT)
                continue;

            if(surface->isIn(position) == EPointPosition::IN)
            {
                return prop->material();
            }
        }

        return _envMaterial;
    }

    std::shared_ptr<Prop> CpuRaytracerWorker::findNearestProp(
            const Ray& rayPrototype,
            RayHitReport& reportMin)
    {
        std::shared_ptr<Prop> propMin;
        Ray ray(rayPrototype);

        for(const auto& prop : _props)
        {
            const std::shared_ptr<Surface>& surface = prop->surface();

            if(surface.get() == nullptr)
                continue;

            RayHitList reports(reportPool);

            const std::shared_ptr<Surface>& bounds = prop->boundingSurface();
            if(bounds.get() != nullptr && !bounds->intersects(ray, reports))
                continue;

            reports.clear();
            surface->raycast(ray, reports);

            bool isNearer = false;
            RayHitReport* node = reports.head;
            while(node != nullptr)
            {
                if(0.0 < node->distance && node->distance < ray.limit)
                {
                    ray.limit = node->distance;
                    reportMin = *node;
                    isNearer = true;
                }

                node = node->_next;
            }

            if(isNearer)
            {
                propMin = prop;
            }
        }

        return propMin;
    }

    glm::dvec3 CpuRaytracerWorker::fireScreenRay(
            const Raycast& raycast)
    {
        const Ray& ray = raycast.ray;
        const glm::dvec3& intensity = raycast.color;
        double maxIntensity = glm::max(glm::max(
            intensity.r, intensity.g), intensity.b);
        if(maxIntensity < _screenRayIntensityThreshold)
        {
            return glm::dvec3(0.0);
        }

        double maxDist = raycast.material->lightFreePathLength(ray);

        const Coating* dummyCoat = nullptr;
        const glm::dvec3 dummyVec = glm::dvec3();
        RayHitReport reportMin(ray, maxDist, dummyVec, dummyVec, dummyCoat, dummyVec);
        std::shared_ptr<Prop> propMin = findNearestProp(ray, reportMin);

        if(propMin.get() != nullptr || maxDist != INFINITY)
        {
            if(!_useStochasticTracing)
            {
                return draft(raycast, reportMin, propMin);
            }

            unsigned int outRayCountHint = glm::ceil(_diffuseRayCount * maxIntensity);
            glm::dvec3 matAtt = raycast.material->lightAttenuation(ray, reportMin.distance);
            glm::dvec3 totalAttenuation = matAtt * raycast.color;
            std::vector<Raycast> outRaycasts;
            glm::dvec3 color(0.0);

            // TODO: optimise reports generation
            // by skipping intersection past maxDist
            if(reportMin.distance > maxDist)
            {
                raycast.material->scatterLight(
                        outRaycasts,
                        ray,
                        maxDist,
                        raycast.material,
                        outRayCountHint);
            }
            else
            {
                reportMin.compile();
                std::shared_ptr<Material> enteredMaterial =
                        findAmbientMaterial(reportMin.refractionOrigin);

                reportMin.coating->brdf(
                        outRaycasts,
                        reportMin,
                        raycast.material,
                        enteredMaterial,
                        outRayCountHint);
            }

            for(Raycast cast : outRaycasts)
            {
                glm::dvec3 stageColor = cast.color * matAtt;
                cast.color *= totalAttenuation;
                color += fireScreenRay(cast) * stageColor;
            }

            return color;
        }
        else if(_backdrop.get() != nullptr)
        {
            if(intensity != glm::dvec3(1.0) ||
               _backdrop->isDirectlyVisible())
            {
                return _backdrop->raycast(ray);
            }
        }

        return glm::dvec3(0.0);
    }

    glm::dvec3 CpuRaytracerWorker::draft(
        const Raycast& raycast,
        const RayHitReport& report,
        const std::shared_ptr<Prop>& prop)
    {
        const glm::dvec3 sunDir(0.5345, 0.2673, 0.8017);
        double attenuation = glm::dot(report.normal, sunDir);
        attenuation = 0.125 + (attenuation/2 + 0.5) * 0.875;

        return glm::dvec3(attenuation);
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
