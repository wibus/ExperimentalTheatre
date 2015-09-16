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
        _lightRayIntensityThreshold(1.0 / 32.0),
        _screenRayIntensityThreshold(1.0 / 128.0),
        _lightDirectRayCount(1),
        _lightFireRayCount(20),
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
            if(_runningPredicate &&
               _lightRayIntensityThreshold != INFINITY)
                shootFromLights();

            if(_runningPredicate &&
               _screenRayIntensityThreshold != INFINITY)
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
        // !!!! RETURNING !!!!
        return;



        _lightHitReports.clear();
        _lightEnteredMaterials.clear();

        // If non-stochatic draft is active
        if(!_useStochasticTracing)
            return;

        std::vector<Raycast> raycasts =
                _backdrop->fireRays(_lightFireRayCount);

        size_t raycastCount = raycasts.size();
        for(size_t i=0; i < raycastCount; ++i)
        {
            Raycast& ray = raycasts[i];
            ray.material = findAmbientMaterial(ray.origin);

            fireLightRay(ray);

            // Verify if this frame must be skipped
            if(!_runningPredicate)
                return;
        }
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

                // Verify if this frame must be skipped
                if(!_runningPredicate)
                    return;
            }
        }
    }

    void CpuRaytracerWorker::fireLightRay(
            const Raycast& fromLightRay)
    {
        // Check if step contribution is too small
        const glm::dvec3& currAtt = fromLightRay.color;
        double currIntensity = glm::max(glm::max(currAtt.x, currAtt.y), currAtt.z);
        if(currIntensity < _lightRayIntensityThreshold)
        {
            // Generate no more light hit point
            return;
        }


        // Compute maximum travelled distance in current material
        const std::shared_ptr<Material>& material = fromLightRay.material;
        double matPathLen = material->lightFreePathLength(fromLightRay);
        Raycast ray = fromLightRay;
        ray.limit = matPathLen;


        // Find nearest ray-surface intersection
        const Coating* dummyCoat = nullptr;
        const glm::dvec3 dummyVec = glm::dvec3();
        RayHitReport reportMin(matPathLen, fromLightRay, dummyVec,
                               dummyVec, dummyVec, dummyCoat);
        ray.limit = findNearestProp(ray, reportMin);


        if(ray.limit != Raycast::BACKDROP_DISTANCE)
        {
            glm::dvec3 matAtt = material->lightAttenuation(ray);

            std::vector<Raycast> outRaycasts;
            unsigned int outRayCountHint = glm::ceil(
                    _diffuseRayCount * currIntensity);

            if(reportMin.distance >= matPathLen)
            {
                ray.limit = matPathLen;

                // Inderect lighting
                material->scatterLight(
                        outRaycasts,
                        ray,
                        material,
                        outRayCountHint);

                // Adding RayHitReport to lights' hit list
                // Coating == nullptr means ray scattering.
                //  i.e. It did not touch a surface, but have been scattered
                //  somewhere int the current ray's material.
                reportMin.incidentRay = ray;
                _lightHitReports.push_back(reportMin);
                _lightEnteredMaterials.push_back(nullptr);
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

                // Adding RayHitReport to lights' hit list
                _lightHitReports.push_back(reportMin);
                _lightEnteredMaterials.push_back(enteredMaterial);
            }

            for(Raycast& brdf : outRaycasts)
            {
                glm::dvec3 stepAtt = brdf.color * matAtt;
                glm::dvec3 nextAtt = currAtt * stepAtt;
                double nextEntropy = Raycast::mixEntropies(
                        brdf.entropy, fromLightRay.entropy);

                brdf.color = nextAtt;
                brdf.entropy = nextEntropy;
                fireLightRay(brdf);
            }
        }
    }

    glm::dvec3 CpuRaytracerWorker::fireScreenRay(
            const Raycast& fromEyeRay)
    {
        // Check if step contribution is too small
        const glm::dvec3& currAtt = fromEyeRay.color;
        double currIntensity = glm::max(glm::max(currAtt.x, currAtt.y), currAtt.z);
        if(currIntensity < _screenRayIntensityThreshold)
        {
            return glm::dvec3(0.0);
        }


        // Compute maximum travelled distance in current material
        const std::shared_ptr<Material>& material = fromEyeRay.material;
        double matPathLen = material->lightFreePathLength(fromEyeRay);
        Raycast ray = fromEyeRay;
        ray.limit = matPathLen;


        // Find nearest ray-surface intersection
        const Coating* dummyCoat = nullptr;
        const glm::dvec3 dummyVec = glm::dvec3();
        RayHitReport reportMin(matPathLen, fromEyeRay, dummyVec,
                               dummyVec, dummyVec, dummyCoat);
        ray.limit = findNearestProp(ray, reportMin);

        glm::dvec3 matAtt = material->lightAttenuation(ray);

        if(ray.limit != Raycast::BACKDROP_DISTANCE)
        {
            // If non-stochatic draft is active
            if(!_useStochasticTracing)
            {
                return draft(fromEyeRay, reportMin);
            }

            glm::dvec3 toEyeColorSum(0.0);


            std::vector<Raycast> outRaycasts;
            unsigned int outRayCountHint = glm::ceil(
                    _diffuseRayCount * currIntensity);

            if(reportMin.distance >= matPathLen)
            {
                ray.limit = matPathLen;

                // Inderect lighting
                material->scatterLight(
                        outRaycasts,
                        ray,
                        material,
                        outRayCountHint);

                /*
                // Direct lighting
                toEyeColorSum += gatherScatteredLight(
                    *material, ray);
                    * */
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

                /*
                // Direct lighting
                toEyeColorSum += gatherReflectedLight(
                    *coating, *material, reportMin);
                    * */
            }

            for(Raycast& brdf : outRaycasts)
            {
                glm::dvec3 stepAtt = brdf.color * matAtt;
                glm::dvec3 nextAtt = currAtt * stepAtt;
                double nextEntropy =
                    Raycast::mixEntropies(
                        brdf.entropy, fromEyeRay.entropy);

                brdf.color = nextAtt;
                brdf.entropy = nextEntropy;
                toEyeColorSum += fireScreenRay(brdf) * stepAtt;
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

    glm::dvec3 CpuRaytracerWorker::gatherScatteredLight(
            const Material& material,
            const Raycast& outRay)
    {
        glm::dvec3 colorSum;

        glm::dvec3 scatterPoint = outRay.origin + outRay.direction * outRay.limit;
        glm::dvec3 scatterOutDir = -outRay.direction;

        std::vector<Raycast> lightCasts =
            _backdrop->fireOn(scatterPoint, _lightDirectRayCount);
        gatherLightHitsToward(lightCasts, scatterPoint);

        size_t lightCastCount = lightCasts.size();
        for(size_t c=0; c < lightCastCount; ++c)
        {
            Raycast& lightRay = lightCasts[c];

            const Coating* dummyCoat = nullptr;
            const glm::dvec3 dummyVec = glm::dvec3();
            RayHitReport lightReport(Raycast::BACKDROP_DISTANCE, lightRay,
                                     dummyVec, dummyVec, dummyVec, dummyCoat);

            lightRay.limit = findNearestProp(lightRay, lightReport);

            double scatterDist = glm::distance(lightRay.origin, scatterPoint);

            if(scatterDist <= lightRay.limit)
            {
                lightRay.limit = scatterDist;
                double lightPathLen = material.lightFreePathLength(lightRay);

                if(lightPathLen >= lightRay.limit)
                {
                    glm::dvec3 lightAtt = material.lightAttenuation(lightRay);

                    colorSum += lightAtt *
                        material.gatherLight(
                            lightRay,
                            scatterOutDir);
                }
            }
        }

        return colorSum;
    }

    glm::dvec3 CpuRaytracerWorker::gatherReflectedLight(
            const Coating& coating,
            const Material& material,
            const RayHitReport& outReport)
    {
        glm::dvec3 colorSum;

        glm::dvec3 outDirirection = -outReport.incidentRay.direction;

        std::vector<Raycast> lightCasts =
            _backdrop->fireOn(outReport.position, _lightDirectRayCount);
        gatherLightHitsToward(lightCasts, outReport.position);

        size_t lightCastCount = lightCasts.size();
        for(size_t c=0; c < lightCastCount; ++c)
        {
            Raycast& lightRay = lightCasts[c];

            const Coating* dummyCoat = nullptr;
            const glm::dvec3 dummyVec = glm::dvec3();
            RayHitReport lightReport(Raycast::BACKDROP_DISTANCE, lightRay,
                                     dummyVec, dummyVec, dummyVec, dummyCoat);

            lightRay.limit = findNearestProp(lightRay, lightReport);
            if(lightRay.limit != Raycast::BACKDROP_DISTANCE)
            {
                if(glm::length(lightReport.position - outReport.position)
                     < RayHitReport::EPSILON_LENGTH)
                {
                    double lightPathLen = material.lightFreePathLength(lightRay);
                    if(lightPathLen >= lightRay.limit)
                    {
                        glm::dvec3 lightAtt = material.lightAttenuation(lightRay);

                        lightReport.compile();
                        std::shared_ptr<Material> lightEnteredMaterial =
                                findAmbientMaterial(lightReport.refractionOrigin);

                        colorSum += lightAtt *
                            coating.directBrdf(
                                lightReport,
                                outDirirection,
                                _envMaterial,
                                lightEnteredMaterial);
                    }
                }
            }
        }

        return colorSum;
    }

    void CpuRaytracerWorker::gatherLightHitsToward(
            std::vector<Raycast>& outRaycasts,
            const glm::dvec3& targetPos)
    {
        size_t lightHitCount = _lightHitReports.size();
        for(size_t i=0; i < lightHitCount; ++i)
        {
            RayHitReport& lightReport = _lightHitReports[i];

            const Raycast& lightRay = lightReport.incidentRay;

            glm::dvec3 outDir = glm::normalize(
                targetPos - lightReport.position);
            glm::dvec3 outOrig = lightReport.position +
                                 outDir * RayHitReport::EPSILON_LENGTH;

            glm::dvec3 outColor;

            // Light hit is a scatter point
            if(lightReport.coating == nullptr)
            {
                outColor = lightRay.material->gatherLight(
                                lightRay, outDir);
            }
            // Light hit is a surface reflection
            else
            {
                outColor = lightReport.coating->directBrdf(
                                lightReport,
                                outDir,
                                lightRay.material,
                                _lightEnteredMaterials[i]);
            }

            if(outColor != glm::dvec3(0.0))
            {
                outRaycasts.push_back(
                    Raycast(
                        Raycast::BACKDROP_DISTANCE,
                        Raycast::FULLY_DIFFUSIVE_ENTROPY,
                        outColor,
                        outOrig,
                        outDir,
                        lightRay.material));
            }
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
            const Raycast& raycast,
            RayHitReport& reportMin)
    {
        Raycast ray(raycast);

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

            RayHitReport* node = reports.head;
            while(node != nullptr)
            {
                if(0.0 < node->distance && node->distance < ray.limit)
                {
                    ray.limit = node->distance;
                    reportMin = *node;
                }

                node = node->_next;
            }
        }

        return reportMin.distance;
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
