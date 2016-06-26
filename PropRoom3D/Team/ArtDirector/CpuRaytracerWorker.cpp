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

#include "Network/TileMessage.h"


namespace prop3
{
    void CpuRaytracerWorker::launchWorker(
        const std::shared_ptr<CpuRaytracerWorker>& worker)
    {
        worker->execute();
    }

    CpuRaytracerWorker::CpuRaytracerWorker() :
        _runningPredicate(false),
        _terminatePredicate(false),
        _incomingTileOnly(false),
        _useStochasticTracing(true),
        _usePixelJittering(true),
        _useDepthOfField(true),
        _lightRayIntensityThreshold(1.0 / 32.0),
        _lightDirectRayCount(1),
        _lightFireRayCount(20),
        _screenRayIntensityThreshold(1.0 / 16.0),
        _maxScreenBounceCount(24),
        _sufficientScreenRayBounce(4),
        _sufficientScreenRayWeight(0.80),
        _minScreenRayWeight(0.04),
        _aperture(0.0),
        _confusionRadius(0.1)
    {
    }

    CpuRaytracerWorker::~CpuRaytracerWorker()
    {
    }

    void CpuRaytracerWorker::start(bool incomingTileOnly)
    {
        _incomingTileOnly = incomingTileOnly;
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
            std::unique_lock<std::mutex> lk(_flowMutex);
            _cv.wait(lk, [this]{
                if(_terminatePredicate)
                    return true;

                if(_workingFilm->incomingTileAvailable())
                    return true;

                if(_runningPredicate &&
                   !_searchStructure->isEmpty() &&
                   _workingFilm->needNewTiles())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            });

            // Verify if we are supposed to terminate
            if(_terminatePredicate)
            {
                return;
            }

            // Process tiles
            while(_runningPredicate)
            {
                // Process as much incoming tiles as possible
                while(_runningPredicate)
                {
                    std::shared_ptr<TileMessage> msg =
                        _workingFilm->nextIncomingTile();

                    if(msg.get() != nullptr)
                    {
                        msg->decode();
                    }
                    else
                    {
                        if(_incomingTileOnly)
                        {
                            _runningPredicate = false;
                        }

                        break;
                    }
                }

                // Generate a single new tile
                if(_runningPredicate)
                {
                    /*// Shoot rays
                    if(_runningPredicate &&
                       _lightRayIntensityThreshold != INFINITY)
                        shootFromLights();
                    */

                    std::shared_ptr<Tile> tile;
                    tile = _workingFilm->nextTile();
                    if(tile != _workingFilm->endTile())
                    {
                        tile->lock();
                        shootFromScreen(tile);
                        tile->unlock();
                    }
                    else
                    {
                        _runningPredicate = false;
                    }
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

        for(TileIterator it = tile->begin();
            it != tile->end() && _runningPredicate;
            ++it)
        {
            int pixelCycleCount = 0;
            int maxCycleCount = (_useStochasticTracing ? 16 : 1);

            double expextedWeightSum = it.sampleWeight();
            double multipliedWeightSum = expextedWeightSum *
                                         it.sampleMultiplicity();

            double totalWeightSum = 0.0;

            while(_runningPredicate &&
                  totalWeightSum < multipliedWeightSum &&
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

                glm::dvec4 sample = fireScreenRay(raycast);

                if(sample.w > 0.0)
                {
                    it.addSample(sample);
                    totalWeightSum += sample.w;
                }
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
        _workingSample = glm::dvec4(0);

        size_t rayId = 0;
        int bounceCount = 1;
        int rayBatchEnd = 1;
        _rayBounceArray.clear();
        _rayBounceArray.push_back(fromEyeRay);

        while(rayId < _rayBounceArray.size())
        {
            Raycast ray = _rayBounceArray[rayId];

            const Coating* nullCoat = nullptr;
            const Material* nullMat = nullptr;
            const glm::dvec3 nullVec3 = glm::dvec3();
            RayHitReport reportMin(Raycast::BACKDROP_LIMIT,
                    nullVec3, nullVec3, nullVec3, nullCoat, nullMat, nullMat);

            // Find nearest ray-surface intersection
            double hitDistance = _searchStructure->
                findNearestIntersection(ray, reportMin, _rayHitList);
            reportMin.compile(ray.direction);

            // If non-stochatic draft is active
            if(!_useStochasticTracing)
            {
                if(hitDistance == Raycast::BACKDROP_LIMIT)
                {
                    glm::dvec4 sample = _backdrop->raycast(ray);
                    return glm::dvec4(glm::dvec3(sample) / sample.w,
                                      ArtDirectorServer::IMAGE_DEPTH);
                }
                else
                {
                    double depth = hitDistance * glm::dot(ray.direction, _camDir);
                    return glm::dvec4(draft(reportMin), depth);
                }
            }


            // Compute maximum travelled distance in current material
            double matPathLen = Raycast::BACKDROP_LIMIT;
            const Material* currMat = reportMin.currMaterial;
            if(currMat == nullptr)
            {
                currMat = _ambMaterial.get();
                if(glm::length(ray.origin) < _backdrop->distance(ray))
                    matPathLen = currMat->lightFreePathLength(ray);
                if(matPathLen > _backdrop->distance(ray) * 1.5)
                    matPathLen = Raycast::BACKDROP_LIMIT;
            }
            else
            {
                matPathLen = currMat->lightFreePathLength(ray);
            }


            if(matPathLen == Raycast::BACKDROP_LIMIT &&
               hitDistance == Raycast::BACKDROP_LIMIT)
            {
                if(_backdrop.get() != nullptr)
                {
                    Raycast backRay = ray;
                    backRay.pathLength += backRay.limit;
                    backRay.virtDist += backRay.limit * backRay.entropy;
                    backRay.limit = _backdrop->distance(backRay);
                    glm::dvec4 matAtt = currMat->lightAttenuation(backRay);
                    commitSample(backRay.sample * matAtt * _backdrop->raycast(backRay));
                }
            }
            else if(hitDistance < matPathLen)
            {
                Raycast brdfRay = ray;
                brdfRay.limit = hitDistance;
                brdfRay.pathLength += brdfRay.limit;
                brdfRay.virtDist += brdfRay.limit * brdfRay.entropy;

                glm::dvec4 matAtt = currMat->lightAttenuation(brdfRay);
                brdfRay.sample = brdfRay.sample * matAtt;

                const Coating* coating = reportMin.coating;


                // Direct lighting
                gatherReflectedLight(
                    *coating,
                    reportMin,
                    brdfRay);

                // Inderect lighting
                if(bounceCount < _maxScreenBounceCount)
                {
                    commitSample(brdfRay.sample *
                        coating->indirectBrdf(
                            _tempChildRayArray,
                            reportMin,
                            brdfRay));

                    commitBounce(brdfRay);
                }
            }
            else
            {
                Raycast scatterRay = ray;
                scatterRay.limit = matPathLen;
                scatterRay.pathLength += scatterRay.limit;
                scatterRay.virtDist += scatterRay.limit * scatterRay.entropy;

                glm::dvec4 matAtt = currMat->lightAttenuation(scatterRay);
                scatterRay.sample = scatterRay.sample * matAtt;


                // Direct lighting
                gatherScatteredLight(
                    *currMat,
                    scatterRay);

                // Inderect lighting
                if(bounceCount < _maxScreenBounceCount)
                {
                    // Inderect lighting
                    currMat->scatterLight(
                        _tempChildRayArray,
                        scatterRay);

                    commitBounce(scatterRay);
                }
            }


            // Check bounce group end
            if(++rayId == rayBatchEnd)
            {
                // Check if we are satisfied with accumulated samples
                if(bounceCount >= _sufficientScreenRayBounce &&
                   _workingSample.w >= _sufficientScreenRayWeight)
                    break;

                rayBatchEnd = _rayBounceArray.size();
                ++bounceCount;
            }
        }

        return _workingSample;
    }

    void CpuRaytracerWorker::gatherScatteredLight(
            const Material& material,
            const Raycast& outRay)
    {
        if(&material != _ambMaterial.get())
            return;

        glm::dvec3 scattPos = outRay.origin + outRay.direction * outRay.limit;

        _lightRays.clear();
        _backdrop->fireOn(_lightRays, scattPos, _lightDirectRayCount);
        for(const auto& light : _searchStructure->lights())
            light->fireOn(_lightRays, scattPos, _lightDirectRayCount);

        //gatherLightHitsToward(lightCasts, hitReport.position);

        size_t lightCastCount = _lightRays.size();
        for(size_t c=0; c < lightCastCount; ++c)
        {
            LightCast& lightCast = _lightRays[c];
            Raycast& lightRay = lightCast.raycast;

            lightRay.limit = glm::distance(lightRay.origin, scattPos);
            double lightPathLen = material.lightFreePathLength(lightRay);

            if(lightPathLen >= lightRay.limit)
            {
                glm::dvec4 lightAtt = material.lightAttenuation(lightRay);
                glm::dvec4 lighSamp = lightAtt * lightRay.sample;
                glm::dvec4 pathSamp = lighSamp * outRay.sample;

                if(pathSamp.w > _minScreenRayWeight)
                {
                    if(!_searchStructure->intersectsScene(
                            lightRay, _rayHitList, outRay.entropy))
                    {
                        commitSample(pathSamp *
                             material.directBrdf(
                                 lightCast,
                                 outRay));
                    }
                }
            }
        }
    }

    void CpuRaytracerWorker::gatherReflectedLight(
            const Coating& coating,
            const RayHitReport& hitReport,
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
            LightCast& lightCast = _lightRays[c];
            Raycast& lightRay = lightCast.raycast;

            if(glm::dot(lightRay.direction, hitReport.normal) < 0.0)
                lightRay.limit = glm::distance(lightRay.origin, hitReport.reflectionOrigin);
            else
                lightRay.limit = glm::distance(lightRay.origin, hitReport.refractionOrigin);

            RayHitReport shadowReport = hitReport;
            shadowReport.compile(lightRay.direction);
            const Material* currMaterial = shadowReport.currMaterial;

            if(currMaterial != _ambMaterial.get())
                continue;

            double lightPathLen = currMaterial->lightFreePathLength(lightRay);

            if(lightPathLen >= lightRay.limit)
            {
                glm::dvec4 lightAtt = currMaterial->lightAttenuation(lightRay);
                glm::dvec4 lighSamp = lightAtt * lightRay.sample;
                glm::dvec4 pathSamp = lighSamp * outRay.sample;

                if(pathSamp.w > _minScreenRayWeight)
                {
                    if(!_searchStructure->intersectsScene(
                            lightRay, _rayHitList, outRay.entropy))
                    {
                        commitSample(pathSamp *
                             coating.directBrdf(
                                 lightCast,
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
        if(report.coating != nullptr)
        {
            const glm::dvec3 sunDir(0.5345, 0.2673, 0.8017);
            glm::dvec3 albedo = report.coating->albedo(report);
            double attenuation = glm::dot(report.normal, sunDir);
            return albedo * (0.125 + (attenuation/2 + 0.5) * 0.875);
        }

        return color::black;
    }

    inline void CpuRaytracerWorker::commitSample(const glm::dvec4& sample)
    {
        _workingSample += sample;
    }

    inline void CpuRaytracerWorker::commitBounce(
            const Raycast& inRay)
    {
        size_t childCount = _tempChildRayArray.size();
        for(size_t i=0; i < childCount; ++i)
        {
            Raycast& childRay = _tempChildRayArray[i];
            childRay.sample *= inRay.sample;

            if(childRay.sample.a > _minScreenRayWeight)
            {
                glm::dvec3 color = glm::dvec3(childRay.sample) / childRay.sample.a;
                if(color.r > _screenRayIntensityThreshold ||
                   color.g > _screenRayIntensityThreshold ||
                   color.b > _screenRayIntensityThreshold)
                {
                    childRay.entropy = Raycast::mixEntropies(inRay.entropy, childRay.entropy);
                    childRay.pathLength = inRay.pathLength;
                    childRay.virtDist = inRay.virtDist;

                    _rayBounceArray.push_back(childRay);
                }
                else
                {
                   commitSample(glm::dvec4(0, 0, 0, childRay.sample.a / 2.0));
                }
            }
        }

        _tempChildRayArray.clear();
    }
}
