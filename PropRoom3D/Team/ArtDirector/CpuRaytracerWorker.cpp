#include "CpuRaytracerWorker.h"

#include <list>

#include <GLM/gtc/random.hpp>

#include "Prop/Prop.h"
#include "Prop/Surface/Surface.h"
#include "Prop/Coating/Coating.h"
#include "Prop/Material/Material.h"

#include "Ray/Raycast.h"
#include "Ray/RayHitList.h"
#include "Ray/RayHitReport.h"

#include "Light/Backdrop/Backdrop.h"
#include "Light/LightBulb/LightBulb.h"

#include "Node/StageSet.h"
#include "Serial/JsonReader.h"

#include "../AbstractTeam.h"

#include "Film/StaticFilm.h"


namespace prop3
{
    class WorkerTeam : public AbstractTeam
    {
    public:
        WorkerTeam() :
            AbstractTeam(nullptr /* Choreographer */)
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
        _useStochasticTracing(true),
        _usePixelJittering(true),
        _useDepthOfField(true),
        _lightRayIntensityThreshold(1.0 / 32.0),
        _screenRayIntensityThreshold(1.0 / 32.0),
        _lightDirectRayCount(1),
        _lightFireRayCount(20),
        _maxScreenBounceCount(6),
        _viewportSize(1, 1),
        _confusionRadius(0.1),
        _team(new WorkerTeam())
    {
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

            // Wait till this worker terminates
            //std::lock_guard<std::mutex> lk(_flowMutex);
        }

        _cv.notify_one();
    }

    bool CpuRaytracerWorker::isRunning()
    {
        return _runningPredicate;
    }

    void CpuRaytracerWorker::updateStageSet(const std::string& stream)
    {
        skipAndExecute([this, &stream](){
            _stageSetStream = stream;
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

    void CpuRaytracerWorker::useStochasticTracing(bool use)
    {
        _useStochasticTracing = use;
    }

    void CpuRaytracerWorker::usePixelJittering(bool use)
    {
        _usePixelJittering = use;
    }

    void CpuRaytracerWorker::useDepthOfField(bool use)
    {
        _useDepthOfField = use;
    }

    size_t CpuRaytracerWorker::completedFrameCount()
    {
        std::lock_guard<std::mutex> lk(_framesMutex);
        return _completedFilms.size();
    }

    std::shared_ptr<AbstractFilm> CpuRaytracerWorker::readNextFilm()
    {
        _framesMutex.lock();
        return _completedFilms.front();
    }

    void CpuRaytracerWorker::popReadFilm()
    {
        _completedFilms.front()->clear();
        _filmPool.push_back(_completedFilms.front());
        _completedFilms.pop();

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
                return _terminatePredicate ||
                       (_runningPredicate &&
                            (!_searchSurfaces.empty() ||
                             !_stageSetStream.empty()));
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
                _ambMaterial = _team->stageSet()->ambientMaterial();
                _backdrop = _team->stageSet()->backdrop();
                _stageSet = _team->stageSet();
                _stageSetStream.clear();

                compileSearchStructures();
            }

/*
            // Shoot rays
            if(_runningPredicate &&
               _lightRayIntensityThreshold != INFINITY)
                shootFromLights();
*/
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
/*
    void CpuRaytracerWorker::shootFromLights()
    {
        // !!!! RETURNING !!!!
        return;



        _lightHitReports.clear();

        // If non-stochatic draft is active
        if(!_useStochasticTracing)
            return;

        std::vector<Raycast> raycasts =
                _backdrop->fireRays(_lightFireRayCount);

        size_t raycastCount = raycasts.size();
        for(size_t i=0; i < raycastCount; ++i)
        {
            Raycast& ray = raycasts[i];

            fireLightRay(ray);

            // Verify if this frame must be skipped
            if(!_runningPredicate)
                return;
        }
    }
*/
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

        glm::dvec3 eyeWorldPos = _camPos;
        if(_useDepthOfField)
        {
            glm::dvec4 apertureBeg = _projInvMatrix * glm::dvec4(0.0, 0.0, -1.0, 1.0);
            apertureBeg.z /= apertureBeg.w;
            glm::dvec4 apertureEnd = _projInvMatrix * glm::dvec4(0.0, 0.0, 1.0, 1.0);
            apertureEnd.z /= apertureEnd.w;
            double aperture = (apertureBeg.z - apertureEnd.z) - 1.0;

            if(aperture > 0.0)
            {
                glm::dvec3 camDir = glm::dvec3(_viewInvMatrix * glm::dvec4(0.0, 0.0, -1.0, 0.0));
                glm::dvec3 confusionSide = glm::normalize(glm::cross(camDir, glm::dvec3(0.0, 0.0, 1.0)));
                glm::dvec3 confusionUp = glm::normalize(glm::cross(confusionSide, camDir));
                glm::dvec2 confusionPos = glm::diskRand(_confusionRadius * aperture);
                eyeWorldPos += confusionSide * confusionPos.x + confusionUp * confusionPos.y;
            }
        }

        Raycast raycast(
            Raycast::BACKDROP_LIMIT,
            Raycast::INITIAL_DISTANCE,
            glm::dvec4(1.0),
            eyeWorldPos,
            glm::dvec3(0.0));

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

                glm::dvec4 sample = fireScreenRay(raycast);
                _workingFilm->addSample(i, j, sample);

                // Verify if this frame must be skipped
                if(!_runningPredicate)
                    return;
            }
        }
    }

    /*
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


        Raycast ray = fromLightRay;
        ray.limit = Raycast::BACKDROP_DISTANCE;


        // Find nearest ray-surface intersection
        const Coating* dummyCoat = nullptr;
        const Material* dummyMat = nullptr;
        const glm::dvec3 dummyVec = glm::dvec3();
        RayHitReport reportMin(Raycast::BACKDROP_DISTANCE,
                               ray, dummyVec, dummyVec, dummyVec,
                               dummyCoat, dummyMat, dummyMat);
        ray.limit = findNearestProp(ray, reportMin);


        if(ray.limit != Raycast::BACKDROP_DISTANCE)
        {
            reportMin.compile();

            // Compute maximum travelled distance in current material
            const Material* currMat = reportMin.currMaterial;
            double matPathLen = currMat->lightFreePathLength(ray);
            ray.limit = glm::min(matPathLen, ray.limit);

            glm::dvec3 matAtt = currMat->lightAttenuation(ray);

            std::vector<Raycast> outRaycasts;
            unsigned int outRayCountHint = glm::ceil(
                    _diffuseRayCount * currIntensity);

            if(reportMin.distance >= matPathLen)
            {
                ray.limit = matPathLen;

                // Inderect lighting
                currMat->scatterLight(
                        outRaycasts,
                        ray,
                        outRayCountHint);

                // Adding RayHitReport to lights' hit list
                // Coating == nullptr means ray scattering.
                //  i.e. It did not touch a surface, but have been scattered
                //  somewhere int the current ray's material.
                reportMin.incidentRay = ray;
                _lightHitReports.push_back(reportMin);
            }
            else
            {
                const Coating* coating = reportMin.coating;
                const Material* nextMat = reportMin.nextMaterial;

                // Inderect lighting
                coating->indirectBrdf(
                        outRaycasts,
                        reportMin,
                        *currMat,
                        *nextMat,
                        outRayCountHint);

                // Adding RayHitReport to lights' hit list
                _lightHitReports.push_back(reportMin);
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
*/
    glm::dvec4 CpuRaytracerWorker::fireScreenRay(
            const Raycast& fromEyeRay)
    {
        int bounceCount = 1;
        int rayBatchEnd = 1;
        _rayBounceArray.clear();
        _rayBounceArray.push_back(fromEyeRay);

        size_t rayId = 0;
        glm::dvec4 sampleAccum(0.0);
        while(rayId < _rayBounceArray.size())
        {
            Raycast ray = _rayBounceArray[rayId];
            ray.limit = Raycast::BACKDROP_LIMIT;


            // Find nearest ray-surface intersection
            const Coating* dummyCoat = nullptr;
            const Material* dummyMat = nullptr;
            const glm::dvec3 dummyVec = glm::dvec3();
            RayHitReport reportMin(Raycast::BACKDROP_LIMIT,
                                   ray, dummyVec, dummyVec, dummyVec,
                                   dummyCoat, dummyMat, dummyMat);
            ray.limit = findNearestIntersection(ray, reportMin);


            if(reportMin.innerMat == Surface::ENVIRONMENT_MATERIAL.get() ||
               reportMin.innerMat == nullptr)
                reportMin.innerMat = _ambMaterial.get();

            if(reportMin.outerMat == Surface::ENVIRONMENT_MATERIAL.get() ||
               reportMin.outerMat == nullptr)
                reportMin.outerMat = _ambMaterial.get();

            reportMin.compile();

            // Compute maximum travelled distance in current material
            const Material* currMat = reportMin.currMaterial;
            double matPathLen = currMat->lightFreePathLength(ray);
            ray.limit = glm::min(matPathLen, ray.limit);

            double currDist = ray.distance + ray.limit;
            double distProb = 1.0 / (currDist * currDist);

            glm::dvec3 matAtt = currMat->lightAttenuation(ray);
            glm::dvec4 currSamp = ray.sample * glm::dvec4(matAtt, 1.0);

            if(ray.limit != Raycast::BACKDROP_LIMIT)
            {
                // If non-stochatic draft is active
                if(!_useStochasticTracing)
                {
                    return glm::dvec4(draft(reportMin), 1.0);
                }

                _tempChildRayArray.clear();
                if(matPathLen < reportMin.distance)
                {
                    // Inderect lighting
                    currMat->scatterLight(
                            _tempChildRayArray,
                            ray);

                    /*
                    // Direct lighting
                    toEyeColorSum += gatherScatteredLight(
                        *material, ray);
                        * */
                }
                else
                {
                    const Coating* coating = reportMin.coating;
                    const Material* nextMat = reportMin.nextMaterial;

                    /*
                    double normalProb = -glm::dot(ray.direction, reportMin.normal);
                    currSamp *= normalProb;
                    */

                    // Inderect lighting
                    sampleAccum += currSamp * distProb *
                        coating->indirectBrdf(
                            _tempChildRayArray,
                            reportMin,
                            *currMat,
                            *nextMat);

                    // Direct lighting
                    /*
                    sampleAccum += gatherReflectedLight(
                        *coating, *nextMat, reportMin);
                    */

                }

                if(bounceCount < _maxScreenBounceCount)
                {
                    size_t childCount = _tempChildRayArray.size();
                    for(size_t i=0; i < childCount; ++i)
                    {
                        Raycast& childRay = _tempChildRayArray[i];
                        childRay.distance = currDist;
                        childRay.sample *= currSamp;

                        double a = childRay.sample.a;
                        if(a <= 0.0)
                            continue;

                        double r = childRay.sample.r;
                        double g = childRay.sample.g;
                        double b = childRay.sample.b;
                        double nextIntensity = glm::max(glm::max(r, g), b) / a;
                        if(nextIntensity > _screenRayIntensityThreshold)
                        {
                            _rayBounceArray.push_back(childRay);
                        }
                    }
                }
            }
            else if(_backdrop.get() != nullptr)
            {
                sampleAccum += currSamp * _backdrop->raycast(ray);
            }


            // Check bounce group end
            if(++rayId == rayBatchEnd)
            {
                ++bounceCount;
                rayBatchEnd = _rayBounceArray.size();
            }
        }

        return sampleAccum;
    }
/*
    glm::dvec3 CpuRaytracerWorker::gatherScatteredLight(
            const Material& material,
            const Raycast& outRay)
    {
        glm::dvec3 colorSum;

        glm::dvec3 scatterPoint = outRay.origin + outRay.direction * outRay.limit;
        glm::dvec3 scatterOutDir = -outRay.direction;

        std::vector<Raycast> lightCasts =
            _backdrop->fireOn(scatterPoint, _lightDirectRayCount);
        //gatherLightHitsToward(lightCasts, scatterPoint);

        size_t lightCastCount = lightCasts.size();
        for(size_t c=0; c < lightCastCount; ++c)
        {
            Raycast& lightRay = lightCasts[c];

            const Coating* dummyCoat = nullptr;
            const Material* dummyMat = nullptr;
            const glm::dvec3 dummyVec = glm::dvec3();
            RayHitReport lightReport(Raycast::BACKDROP_DISTANCE,
                                     lightRay, dummyVec, dummyVec, dummyVec,
                                     dummyCoat, dummyMat, dummyMat);

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
*/
    glm::dvec4 CpuRaytracerWorker::gatherReflectedLight(
            const Coating& coating,
            const Material& material,
            const RayHitReport& hitReport)
    {
        glm::dvec4 sampleSum;

        double baseDist = hitReport.incidentRay.distance;
        glm::dvec3 outDirirection = -hitReport.incidentRay.direction;

        std::vector<Raycast> lightCasts =
            _backdrop->fireOn(hitReport.position, _lightDirectRayCount);
        //gatherLightHitsToward(lightCasts, hitReport.position);

        size_t lightCastCount = lightCasts.size();
        for(size_t c=0; c < lightCastCount; ++c)
        {
            Raycast& lightRay = lightCasts[c];

            const Coating* dummyCoat = nullptr;
            const Material* dummyMat = nullptr;
            const glm::dvec3 dummyVec = glm::dvec3();
            RayHitReport lightReport(Raycast::BACKDROP_LIMIT,
                                     lightRay, dummyVec, dummyVec, dummyVec,
                                     dummyCoat, dummyMat, dummyMat);

            lightRay.limit = findNearestIntersection(lightRay, lightReport);
            if(lightRay.limit != Raycast::BACKDROP_LIMIT)
            {
                if(glm::length(lightReport.position - hitReport.position)
                     < RayHitReport::EPSILON_LENGTH)
                {
                    double lightPathLen = material.lightFreePathLength(lightRay);
                    if(lightPathLen >= lightRay.limit)
                    {
                        double totalDist = baseDist + lightRay.distance + lightRay.limit;
                        double distProb = 1.0 / (totalDist * totalDist);
                        double prob = distProb;

                        glm::dvec3 lightAtt = material.lightAttenuation(lightRay);
                        glm::dvec4 currSamp = glm::dvec4(lightAtt * prob, prob);

                        lightReport.compile();
                        const Material* lightCurrMat = lightReport.currMaterial;
                        const Material* lightNextMat = lightReport.nextMaterial;

                        sampleSum += currSamp *
                            coating.directBrdf(
                                lightReport,
                                outDirirection,
                                *lightCurrMat,
                                *lightNextMat);
                    }
                }
            }
        }

        return sampleSum;
    }
/*
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
                outColor = lightReport.currMaterial->gatherLight(
                                lightRay, outDir);
            }
            // Light hit is a surface reflection
            else
            {
                outColor = lightReport.coating->directBrdf(
                                lightReport,
                                outDir,
                                *lightReport.currMaterial,
                                *lightReport.nextMaterial);
            }

            if(outColor != glm::dvec3(0.0))
            {
                outRaycasts.push_back(
                    Raycast(
                        Raycast::BACKDROP_DISTANCE,
                        Raycast::FULLY_DIFFUSIVE_ENTROPY,
                        outColor,
                        outOrig,
                        outDir));
            }
        }
    }
*/

    struct SearchZone
    {
        size_t parent;
        size_t endZone;
        size_t begLight;
        size_t endLight;
        size_t begSurf;
        size_t endSurf;

        std::shared_ptr<Surface> bounds;
    };

    void CpuRaytracerWorker::compileSearchStructures()
    {
        _searchLights.clear();
        _searchZones.clear();
        _searchSurfaces.clear();

        if(!_stageSet->isVisible())
            return;

        std::vector<std::pair<StageZone*, size_t>> zoneStack;
        zoneStack.push_back(std::make_pair(_stageSet.get(), -1));
        while(!zoneStack.empty())
        {
            StageZone* zone = zoneStack.back().first;
            size_t parentId = zoneStack.back().second;
            zoneStack.pop_back();

            size_t addedSubzones = 0;
            for(size_t s=0; s < zone->subzones().size(); ++s)
            {
                StageZone* subz = zone->subzones()[s].get();

                if(!subz->isVisible())
                    continue;

                // Bubble up unbounded subzones
                if(subz->bounds() == StageZone::UNBOUNDED)
                {
                    // Bubble up props
                    size_t propCount = subz->props().size();
                    for(size_t p=0; p < propCount; ++p)
                    {
                        auto prop = subz->props()[p];
                        if(prop->isVisible())
                            zone->addProp(subz->props()[p]);
                    }

                    // Bubble up lights
                    size_t lightCount = subz->lights().size();
                    for(size_t l=0; l < lightCount; ++l)
                    {
                        auto light = subz->lights()[l];
                        if(light->isOn())
                            zone->addLight(light);
                    }

                    // Bubble up subzones
                    size_t subzCount = subz->subzones().size();
                    for(size_t z=0; z < subzCount; ++z)
                    {
                        auto subsubz = subz->subzones()[z];
                        if(subsubz->isVisible())
                            zone->addSubzone(subsubz);
                    }
                }
                else
                {
                    size_t currId = _searchZones.size();
                    zoneStack.push_back(std::make_pair(subz, currId));
                    ++addedSubzones;
                }
            }


            size_t lightCount = zone->lights().size();
            size_t startLightCount = _searchLights.size();
            for(size_t l=0; l < lightCount; ++l)
            {
                _searchLights.push_back(zone->lights()[l]);
            }
            size_t endLightCount = _searchLights.size();
            size_t addedLights = endLightCount - startLightCount;


            size_t propCount = zone->props().size();
            size_t startSurfCount = _searchSurfaces.size();
            for(size_t p=0; p < propCount; ++p)
            {
                auto prop = zone->props()[p];
                if(prop->isVisible())
                {
                    size_t surfCount = prop->surfaces().size();
                    for(size_t s=0; s < surfCount; ++s)
                    {
                        _searchSurfaces.push_back(prop->surfaces()[s]);
                    }
                }
            }
            size_t endSurfCount = _searchSurfaces.size();
            size_t addedSurfaces = endSurfCount - startSurfCount;


            if(addedSubzones == 0 && addedLights == 0 && addedSurfaces == 0)
                continue;

            SearchZone searchZone;
            searchZone.parent = parentId;
            searchZone.endZone = _searchZones.size() + 1;
            searchZone.endLight = _searchLights.size();
            searchZone.begLight = searchZone.endLight - addedLights;
            searchZone.endSurf = _searchSurfaces.size();
            searchZone.begSurf = searchZone.endSurf - addedSurfaces;
            searchZone.bounds = zone->bounds();
            _searchZones.push_back(searchZone);
        }

		int last = int(_searchZones.size() - 1);
		for(int i = last; i >= 0; --i)
        {
            const SearchZone& zone = _searchZones[i];
            if(zone.parent != -1)
            {
                SearchZone& parent = _searchZones[zone.parent];
                parent.endZone = glm::max(parent.endZone, zone.endZone);
                parent.endLight = glm::max(parent.endLight, zone.endLight);
                parent.endSurf = glm::max(parent.endSurf, zone.endSurf);
            }
        }
    }

    double CpuRaytracerWorker::findNearestIntersection(
            const Raycast& raycast,
            RayHitReport& reportMin)
    {
        Raycast ray(raycast);

        size_t zId = 0;
        size_t zoneCount = _searchZones.size();
        while(zId < zoneCount)
        {
            const SearchZone& zone = _searchZones[zId];

            if(zone.begSurf != zone.endSurf ||
               zone.begLight != zone.endLight)
            {
                if(zone.bounds == StageZone::UNBOUNDED ||
                   zone.bounds->intersects(ray, _rayHitList))
                {
                    for(size_t s = zone.begSurf; s < zone.endSurf; ++s)
                    {
                        _rayHitList.clear();

                        _searchSurfaces[s]->raycast(ray, _rayHitList);

                        RayHitReport* node = _rayHitList.head;
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

                    for(size_t l = zone.begLight; l < zone.endLight; ++l)
                    {
                        _rayHitList.clear();

                        _searchLights[l]->raycast(ray, _rayHitList);

                        RayHitReport* node = _rayHitList.head;
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

                    ++zId;
                }
                else
                {
                    zId = zone.endZone;
                }
            }
            else
            {
                ++zId;
            }
        }


        return reportMin.distance;
    }

    glm::dvec3 CpuRaytracerWorker::draft(
        const RayHitReport& report)
    {
        const glm::dvec3 sunDir(0.5345, 0.2673, 0.8017);
        glm::dvec3 albedo = report.coating->albedo(report);
        double attenuation = glm::dot(report.normal, sunDir);
        attenuation = 0.125 + (attenuation/2 + 0.5) * 0.875;

        return albedo * attenuation;
    }

    void CpuRaytracerWorker::resetBuffers()
    {
        while(!_completedFilms.empty())
        {
            _completedFilms.front()->clear();
            _filmPool.push_back(_completedFilms.front());
            _completedFilms.pop();
        }

        if(_workingFilm.get() != nullptr)
        {
            _workingFilm->clear();
        }
    }

    void CpuRaytracerWorker::destroyBuffers()
    {
        while(!_completedFilms.empty())
            _completedFilms.pop();

        _workingFilm.reset();
        _filmPool.clear();
    }

    void CpuRaytracerWorker::getNewWorkingBuffers()
    {
        if(_filmPool.empty())
        {
            _workingFilm.reset(new StaticFilm());
            _workingFilm->resize(_viewportSize);
        }
        else
        {
            _workingFilm = _filmPool.back();
            _filmPool.pop_back();
        }
    }

    void CpuRaytracerWorker::commitWorkingBuffers()
    {
        _framesMutex.lock();
        _completedFilms.push(
            _workingFilm);
        _framesMutex.unlock();

        getNewWorkingBuffers();
    }
}
