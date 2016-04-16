#include "CpuRaytracerWorker.h"

#include <list>
#include <iostream>

#include "Node/Prop/Prop.h"
#include "Node/Prop/Surface/Surface.h"
#include "Node/Prop/Coating/Coating.h"
#include "Node/Prop/Material/Material.h"

#include "Ray/Raycast.h"
#include "Ray/RayHitList.h"
#include "Ray/RayHitReport.h"
#include "SearchStructure.h"

#include "Node/Light/LightCast.h"
#include "Node/Light/Backdrop/Backdrop.h"
#include "Node/Light/LightBulb/LightBulb.h"

#include "Node/StageSet.h"

#include "../AbstractTeam.h"
#include "ArtDirectorServer.h"

#include "Film/StaticFilm.h"


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
        _useStochasticTracing(true),
        _usePixelJittering(true),
        _useDepthOfField(true),
        _lightRayIntensityThreshold(1.0 / 32.0),
        _lightDirectRayCount(1),
        _lightFireRayCount(20),
        _screenRayIntensityThreshold(1.0/64.0),
        _maxScreenBounceCount(24),
        _sufficientScreenRayBounce(5),
        _sufficientScreenRayWeight(0.75),
        _minScreenRayWeight(0.04),
        _aperture(0.0),
        _confusionRadius(0.1)
    {
    }

    CpuRaytracerWorker::~CpuRaytracerWorker()
    {
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
        }

        _cv.notify_one();
    }

    bool CpuRaytracerWorker::isRunning()
    {
        return _runningPredicate;
    }

    void CpuRaytracerWorker::waitForStop()
    {
        std::lock_guard<std::mutex> lk(_flowMutex);
    }

    void CpuRaytracerWorker::updateView(const glm::dmat4& view)
    {
        skipAndExecute([this, &view](){
            _viewInvMatrix = glm::inverse(view);
            _viewProjInverse = _viewInvMatrix * _projInvMatrix;
            _camPos = glm::dvec3(_viewInvMatrix * glm::dvec4(0, 0, 0, 1));

            _camDir = glm::normalize(glm::dvec3(_viewInvMatrix * glm::dvec4(0.0, 0.0, -1.0, 0.0)));
            _confusionSide = glm::normalize(glm::cross(_camDir, glm::dvec3(0.0, 0.0, 1.0)));
            _confusionUp = glm::normalize(glm::cross(_confusionSide, _camDir));
        });
    }

    void CpuRaytracerWorker::updateProjection(const glm::dmat4& proj)
    {
        skipAndExecute([this, &proj](){
            _projInvMatrix = glm::inverse(proj);
            _viewProjInverse = _viewInvMatrix * _projInvMatrix;

            glm::dvec4 apertureBeg = _projInvMatrix * glm::dvec4(0.0, 0.0, -1.0, 1.0);
            apertureBeg.z /= apertureBeg.w;
            glm::dvec4 apertureEnd = _projInvMatrix * glm::dvec4(0.0, 0.0, 1.0, 1.0);
            apertureEnd.z /= apertureEnd.w;
            _aperture = _confusionRadius * ((apertureBeg.z - apertureEnd.z) -
                                            ArtDirectorServer::IMAGE_DEPTH);
        });
    }

    void CpuRaytracerWorker::updateFilm(std::shared_ptr<Film>& film)
    {
        skipAndExecute([this, &film](){
            _workingFilm = film;
        });
    }

    void CpuRaytracerWorker::updateSearchStructure(
            const std::shared_ptr<SearchStructure>& searchStructure)
    {
        skipAndExecute([this, &searchStructure](){
            _stageSet = searchStructure->team()->stageSet();
            _backdrop = _stageSet->backdrop();
            _ambMaterial = _stageSet->ambientMaterial();
            _searchStructure = searchStructure;
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

    void CpuRaytracerWorker::skipAndExecute(const std::function<void()>& func)
    {
        // Skip current frame
        bool isRunning = _runningPredicate;
        _runningPredicate = false;

        // Lock and execute
        std::lock_guard<std::mutex> lk(_flowMutex);
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
            std::shared_ptr<Tile> tile;
            std::unique_lock<std::mutex> lk(_flowMutex);
            _cv.wait(lk, [this, &tile]{
                if(_terminatePredicate)
                    return true;

                if(_runningPredicate &&
                    (!_searchStructure->isEmpty()))
                {
                    tile = _workingFilm->nextTile();
                    return tile != _workingFilm->endTile();
                }
                else
                    return false;
            });

            // Verify if we are supposed to terminate
            if(_terminatePredicate)
            {
                return;
            }

            /*// Shoot rays
            if(_runningPredicate &&
               _lightRayIntensityThreshold != INFINITY)
                shootFromLights();
            */

            while(tile != _workingFilm->endTile())
            {
                tile->lock();
                shootFromScreen(tile);
                tile->unlock();

                if(_runningPredicate)
                    tile = _workingFilm->nextTile();
                else
                    break;
            }


            // Stop if single shot
            if(_isSingleShot)
            {
                _runningPredicate = false;
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
    void CpuRaytracerWorker::shootFromScreen(std::shared_ptr<Tile>& tile)
    {
        double pixelWidth = 2.0 / _workingFilm->frameWidth();
        double pixelHeight = 2.0 / _workingFilm->frameHeight();
        glm::dvec2 pixelSize(pixelWidth, pixelHeight);
        glm::dvec2 frameOrig = -glm::dvec2(_workingFilm->frameResolution()) / 2.0;

        if(_usePixelJittering)
        {
            frameOrig += _linearRand.gen2() - glm::dvec2(0.5);
        }

        Raycast raycast(
            Raycast::FULLY_SPECULAR,
            glm::dvec4(1.0),
            _camPos,
            glm::dvec3(0.0));

        int pixId = 0;
        for(TileIterator it = tile->begin();
            it != tile->end() && _runningPredicate;
            ++it, ++pixId)
        {
            glm::dvec4 sampleSum(0.0);
            double expextedWeightSum =
                it.resquestedSampleWeight();

            int pixelCycleCount = 0;
            int maxCycleCount = (_useStochasticTracing ? 5 : 1);

            while(_runningPredicate &&
                  sampleSum.w < expextedWeightSum &&
                  ++pixelCycleCount <= maxCycleCount)
            {
                if(_useDepthOfField && _aperture > 0.0)
                {
                    glm::dvec2 confusionPos = _diskRand.gen(_aperture);
                    raycast.origin = _camPos +
                        _confusionSide * confusionPos.x +
                        _confusionUp * confusionPos.y;
                }

                glm::dvec2 pixPos = glm::dvec2(it.position());
                glm::dvec4 screenPos((frameOrig + pixPos)*pixelSize, -1.0, 1.0);
                glm::dvec4 dirH = _viewProjInverse * screenPos;
                glm::dvec3 pixWorldPos = glm::dvec3(dirH / dirH.w);
                raycast.direction = glm::normalize(pixWorldPos - raycast.origin);
                raycast.invDir = 1.0 / raycast.direction;

                sampleSum += fireScreenRay(raycast);
            }


            if(sampleSum.w > 0.0)
                it.addSample(sampleSum);
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
        _workingSample = glm::dvec4(0);

        size_t rayId = 0;
        int bounceCount = 1;
        int rayBatchEnd = 1;
        _rayBounceArray.clear();
        _rayBounceArray.push_back(fromEyeRay);

        while(rayId < _rayBounceArray.size())
        {
            Raycast ray = _rayBounceArray[rayId];

            const Coating* zCoat = nullptr;
            const Material* zMat = nullptr;
            const glm::dvec3 zVec3 = glm::dvec3();
            RayHitReport reportMin(Raycast::BACKDROP_LIMIT,
                    zVec3, zVec3, zVec3, zCoat, zMat, zMat);

            // Find nearest ray-surface intersection
            double hitDistance = _searchStructure->
                findNearestIntersection(ray, reportMin, _rayHitList);
            ray.limit = hitDistance;

            if(reportMin.innerMat == Surface::ENVIRONMENT_MATERIAL.get() ||
               reportMin.innerMat == nullptr)
                    reportMin.innerMat = _ambMaterial.get();

            if(reportMin.outerMat == Surface::ENVIRONMENT_MATERIAL.get() ||
               reportMin.outerMat == nullptr)
                    reportMin.outerMat = _ambMaterial.get();

            reportMin.compile(ray.direction);


            // Compute maximum travelled distance in current material
            const Material* currMat = reportMin.currMaterial;
            double matPathLen = currMat->lightFreePathLength(ray);
            ray.limit = glm::min(matPathLen, hitDistance);

            glm::dvec3 matAtt = currMat->lightAttenuation(ray);
            glm::dvec4 currSamp = ray.sample * glm::dvec4(matAtt, 1.0);
            ray.virtDist += ray.limit * ray.entropy;
            ray.pathLength += ray.limit;

            if(ray.limit != Raycast::BACKDROP_LIMIT)
            {
                // If non-stochatic draft is active
                if(!_useStochasticTracing)
                {
                    double depth = ray.limit * glm::dot(ray.direction, _camDir);
                    return glm::dvec4(draft(reportMin), depth);
                }

                _tempChildRayArray.clear();
                if(matPathLen < hitDistance)
                {
                    // Inderect lighting
                    currMat->scatterLight(
                        _tempChildRayArray,
                        ray);

                    /*
                    // Direct lighting
                    commitSample(currSamp *
                        gatherScatteredLight(
                            *currMat,
                            ray);
                    */
                }
                else
                {
                    const Coating* coating = reportMin.coating;

                    // Inderect lighting
                    commitSample(currSamp *
                        coating->indirectBrdf(
                            _tempChildRayArray,
                            reportMin,
                            ray));

                    // Direct lighting
                    gatherReflectedLight(
                        *coating,
                        reportMin,
                        currSamp,
                        ray);
                }

                if(bounceCount < _maxScreenBounceCount)
                {
                    size_t childCount = _tempChildRayArray.size();
                    for(size_t i=0; i < childCount; ++i)
                    {
                        Raycast& childRay = _tempChildRayArray[i];
                        childRay.sample *= currSamp;

                        if(childRay.sample.a > _minScreenRayWeight)
                        {
                            glm::dvec3 color = glm::dvec3(childRay.sample) / childRay.sample.a;
                            if(color.r > _screenRayIntensityThreshold ||
                               color.g > _screenRayIntensityThreshold ||
                               color.b > _screenRayIntensityThreshold)
                            {
                                childRay.entropy = Raycast::mixEntropies(ray.entropy, childRay.entropy);
                                childRay.pathLength = ray.pathLength;
                                childRay.virtDist = ray.virtDist;

                                _rayBounceArray.push_back(childRay);
                            }
                            else
                            {
                               commitSample(glm::dvec4(0, 0, 0, childRay.sample.a / 2.0));
                            }
                        }
                    }
                }
            }
            else if(_backdrop.get() != nullptr)
            {
                if(_useStochasticTracing)
                {
                    commitSample(currSamp * _backdrop->raycast(ray));
                }
                else
                {
                    glm::dvec4 sample = currSamp * _backdrop->raycast(ray);
                    return glm::dvec4(
                        glm::dvec3(sample) / sample.w,
                        ArtDirectorServer::IMAGE_DEPTH);
                }
            }


            // Check bounce group end
            if(++rayId == rayBatchEnd)
            {
                // Check if we are satisfied with accumulated samples
                if(bounceCount >= _sufficientScreenRayBounce &&
                   _workingSample.w >= _sufficientScreenRayWeight)
                    break;

                ++bounceCount;
                rayBatchEnd = _rayBounceArray.size();
            }
        }

        return _workingSample;
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
    void CpuRaytracerWorker::gatherReflectedLight(
            const Coating& coating,
            const RayHitReport& hitReport,
            const glm::dvec4& outSample,
            const Raycast& outRay)
    {
        _lightRays.clear();        
        _backdrop->fireOn(_lightRays, hitReport.position, _lightDirectRayCount);
        for(const auto& light : _searchStructure->lights())
            light->fireOn(_lightRays, hitReport.position, _lightDirectRayCount);

        //gatherLightHitsToward(lightCasts, hitReport.position);

        size_t lightCastCount = _lightRays.size();
        for(size_t c=0; c < lightCastCount; ++c)
        {
            LightCast& lightRay = _lightRays[c];
            Raycast& lightCast = lightRay.raycast;

            if(glm::dot(lightCast.direction, hitReport.normal) < 0.0)
                lightCast.limit = glm::distance(lightCast.origin, hitReport.reflectionOrigin);
            else
                lightCast.limit = glm::distance(lightCast.origin, hitReport.refractionOrigin);

            RayHitReport shadowReport = hitReport;
            shadowReport.compile(lightCast.direction);
            const Material& currMaterial = *shadowReport.currMaterial;
            double lightPathLen = currMaterial.lightFreePathLength(lightCast);

            if(lightPathLen >= lightCast.limit)
            {
                glm::dvec3 lightAtt = currMaterial.lightAttenuation(lightCast);
                glm::dvec4 lighSamp = glm::dvec4(lightAtt, 1.0) * lightCast.sample;

                glm::dvec4 pathSamp = lighSamp * outSample;
                if(pathSamp.w > _minScreenRayWeight)
                {
                    if(!_searchStructure->intersectsScene(
                            lightCast, _rayHitList, outRay.entropy))
                    {
                        commitSample(pathSamp *
                             coating.directBrdf(
                                 lightRay,
                                 shadowReport,
                                 outRay));
                    }
                }
            }
        }
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
    glm::dvec3 CpuRaytracerWorker::draft(
        const RayHitReport& report)
    {
        const glm::dvec3 sunDir(0.5345, 0.2673, 0.8017);
        glm::dvec3 albedo = report.coating->albedo(report);
        double attenuation = glm::dot(report.normal, sunDir);
        attenuation = 0.125 + (attenuation/2 + 0.5) * 0.875;

        return albedo * attenuation;
    }

    inline void CpuRaytracerWorker::commitSample(const glm::dvec4& sample)
    {
        _workingSample += sample;
    }
}
