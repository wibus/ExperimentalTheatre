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

#include "../../Prop/Environment/Environment.h"
#include "../../Prop/Environment/Backdrop/Backdrop.h"

#include "../../StageSet/StageSet.h"
#include "../../StageSet/StageSetJsonReader.h"

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
        _backdropRayCount(1),
        _diffuseRayCount(1),
        _viewportSize(1, 1),
        _confusionRadius(0.1),
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

    void CpuRaytracerWorker::setStageSetStream(const std::string& stream)
    {
        skipAndExecute([this, &stream](){
            _stageSetStream = stream;
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
                             !_stageSetStream.empty())) ||
                        _terminatePredicate;
            });

            // Verify if we are supposed to terminate
            if(_terminatePredicate)
            {
                return;
            }

            // Verify if stageSet stream was updated
            if(!_stageSetStream.empty())
            {
                StageSetJsonReader reader;
                reader.deserialize(*_team, _stageSetStream);
                _envMaterial = _team->stageSet()->environment()->ambientMaterial();
                _backdrop = _team->stageSet()->environment()->backdrop();
                _props = _team->stageSet()->props();
                _stageSetStream.clear();
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

        glm::dvec4 apertureBeg = _projInvMatrix * glm::dvec4(0.0, 0.0, -1.0, 1.0);
        apertureBeg.z /= apertureBeg.w;
        glm::dvec4 apertureEnd = _projInvMatrix * glm::dvec4(0.0, 0.0, 1.0, 1.0);
        apertureEnd.z /= apertureEnd.w;
        double aperture = (apertureBeg.z - apertureEnd.z) - 1.0;

        glm::dvec3 eyeWorldPos = _camPos;
        if(aperture > 0.0)
        {
            glm::dvec3 camDir = glm::dvec3(_viewInvMatrix * glm::dvec4(0.0, 0.0, -1.0, 0.0));
            glm::dvec3 confusionSide = glm::normalize(glm::cross(camDir, glm::dvec3(0.0, 0.0, 1.0)));
            glm::dvec3 confusionUp = glm::normalize(glm::cross(confusionSide, camDir));
            glm::dvec2 confusionPos = glm::diskRand(_confusionRadius * aperture);
            eyeWorldPos += confusionSide * confusionPos.x + confusionUp * confusionPos.y;
        }

        Raycast raycast(
            Raycast::BACKDROP_DISTANCE,
            Raycast::FULLY_SPECULAR_ENTROPY,
            glm::dvec3(1.0),
            eyeWorldPos,
            glm::dvec3(0.0),
            ambientMaterial);

        int idx = -1;
        glm::dvec4 screenPos(orig, -1.0, 1);
        for(int j=0; j<_viewportSize.y; ++j, screenPos.y += pixelHeight)
        {
            screenPos.x = orig.x;
            for(int i=0; i<_viewportSize.x; ++i, screenPos.x += pixelWidth)
            {
                glm::dvec4 dirH = _viewProjInverse * screenPos;
                glm::dvec3 pixWorldPos = glm::dvec3(dirH / dirH.w);
                raycast.direction = glm::normalize(pixWorldPos - raycast.origin);


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

    double CpuRaytracerWorker::findNearestProp(
            const Raycast& rayPrototype,
            RayHitReport& reportMin)
    {
        std::shared_ptr<Prop> propMin;
        Raycast ray(rayPrototype);

        for(const auto& prop : _props)
        {
            const std::shared_ptr<Surface>& surface = prop->surface();

            if(surface.get() == nullptr)
                continue;

            RayHitList reports(_reportPool);

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

        return reportMin.distance;
    }

    glm::dvec3 CpuRaytracerWorker::fireScreenRay(
            const Raycast& rayPrototype)
    {
        // Check if step contribution is too small
        const glm::dvec3& currAtt = rayPrototype.color;
        double currIntensity = glm::max(glm::max(currAtt.x, currAtt.y), currAtt.z);
        if(currIntensity < _screenRayIntensityThreshold)
        {
            return glm::dvec3(0.0);
        }


        // Compute maximum travelled distance in current material
        Raycast ray = rayPrototype;
        const std::shared_ptr<Material>& material = rayPrototype.material;
        double matPathLen = material->lightFreePathLength(ray);
        ray.limit = matPathLen;


        // Find nearest ray-surface intersection
        const Coating* dummyCoat = nullptr;
        const glm::dvec3 dummyVec = glm::dvec3();
        RayHitReport reportMin(matPathLen, rayPrototype, dummyVec,
                               dummyVec, dummyVec, dummyCoat);
        ray.limit = findNearestProp(ray, reportMin);

        glm::dvec3 matAtt = material->lightAttenuation(ray);

        if(ray.limit != Raycast::BACKDROP_DISTANCE)
        {
            // If non-stochatic draft is active
            if(!_useStochasticTracing)
            {
                return draft(rayPrototype, reportMin);
            }

            glm::dvec3 toEyeColorSum(0.0);


            std::vector<Raycast> outRaycasts;
            unsigned int outRayCountHint = glm::ceil(
                    _diffuseRayCount * currIntensity);

            if(reportMin.distance >= matPathLen)
            {
                material->scatterLight(
                        outRaycasts,
                        ray,
                        matPathLen,
                        material,
                        outRayCountHint);
            }
            else
            {
                reportMin.compile();

                const Coating* coating = reportMin.coating;
                std::shared_ptr<Material> enteredMaterial =
                        findAmbientMaterial(reportMin.refractionOrigin);

                // Inderect lighting
                coating->indirectBrdf(
                        outRaycasts,
                        reportMin,
                        material,
                        enteredMaterial,
                        outRayCountHint);

                // Direct lighting
                std::vector<Raycast> lightCasts = _backdrop->fireOn(
                        reportMin.position, _backdropRayCount);
                size_t lightCastCount = lightCasts.size();
                for(size_t c=0; c < lightCastCount; ++c)
                {
                    Raycast& lightRay = lightCasts[c];

                    RayHitReport lightReport = reportMin;
                    lightReport.distance = Raycast::BACKDROP_DISTANCE;
                    lightRay.limit = findNearestProp(lightRay, lightReport);
                    if(lightRay.limit != Raycast::BACKDROP_DISTANCE)
                    {
                        if(glm::length(lightReport.position - reportMin.position)
                             < RayHitReport::EPSILON_LENGTH)
                        {
                            double lightPathLen = material->lightFreePathLength(lightRay);
                            if(lightPathLen >= lightRay.limit)
                            {
                                glm::dvec3 lightAtt = material->lightAttenuation(lightRay);

                                lightReport.compile();
                                std::shared_ptr<Material> lightEnteredMaterial =
                                        findAmbientMaterial(lightReport.refractionOrigin);

                                toEyeColorSum += lightAtt * lightRay.color *
                                    coating->directBrdf(
                                        lightReport,
                                        -ray.direction,
                                        _envMaterial,
                                        lightEnteredMaterial);
                            }
                        }
                    }
                }
            }

            for(Raycast& brdf : outRaycasts)
            {
                glm::dvec3 stepAtt = brdf.color * matAtt;
                glm::dvec3 nextAtt = currAtt * stepAtt;
                double nextEntropy =
                    Raycast::mixEntropies(
                        brdf.entropy, rayPrototype.entropy);

                Raycast nextEyeRaycast(brdf.limit, nextEntropy, nextAtt, brdf.origin, brdf.direction, brdf.material);
                toEyeColorSum += fireScreenRay(nextEyeRaycast) * stepAtt;
            }

            return toEyeColorSum;
        }
        else if(_backdrop.get() != nullptr)
        {
            bool directView = (currAtt == glm::dvec3(1.0));
            if(!directView ||
               _backdrop->isDirectlyVisible())
            {
                return _backdrop->raycast(ray, directView) * matAtt;
            }
        }

        return glm::dvec3(0.0);
    }

    glm::dvec3 CpuRaytracerWorker::draft(
        const Raycast& raycast,
        const RayHitReport& report)
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
