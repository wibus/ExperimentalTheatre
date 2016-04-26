#ifndef PROPROOM3D_CPURAYTRACERWORKER_H
#define PROPROOM3D_CPURAYTRACERWORKER_H

#include <memory>
#include <vector>

#include <atomic>
#include <mutex>
#include <condition_variable>

#include <functional>

#include <GLM/glm.hpp>

#include <CellarWorkbench/Misc/Distribution.h>

#include <PropRoom3D/Ray/RayHitList.h>


namespace prop3
{
    class Raycast;
    class LightCast;
    class RayHitReport;
    class StageSet;
    class Backdrop;

    class SearchStructure;

    class Film;
    class Tile;
    class TileIterator;



    class PROP3D_EXPORT CpuRaytracerWorker
    {
    public:
        static void launchWorker(
            const std::shared_ptr<CpuRaytracerWorker>& worker);

        CpuRaytracerWorker();
        virtual ~CpuRaytracerWorker();

        // States
        virtual void start(bool incomingTileOnly = false);
        virtual void stop();
        virtual void terminate();
        virtual bool isRunning();
        virtual void waitForStop();

        // Updates
        virtual void updateView(const glm::dmat4& view);
        virtual void updateProjection(const glm::dmat4& proj);
        virtual void updateFilm(std::shared_ptr<Film>& film);
        virtual void updateSearchStructure(
            const std::shared_ptr<SearchStructure>& searchStructure);


        // Modes
        virtual void useStochasticTracing(bool use);
        virtual void usePixelJittering(bool use);
        virtual void useDepthOfField(bool use);

    protected:
        virtual void skipAndExecute(const std::function<void()>& func);

        virtual void execute();
        //virtual void shootFromLights();
        virtual void shootFromScreen(std::shared_ptr<Tile>& tile);

        //virtual void fireLightRay(
        //        const Raycast& fromLightRay);

        virtual glm::dvec4 fireScreenRay(
                const Raycast& fromEyeRay);

        //virtual glm::dvec3 gatherScatteredLight(
        //        const Material& material,
        //        const Raycast& outRay);

        virtual void gatherReflectedLight(
                const Coating& coating,
                const RayHitReport& hitReport,
                const glm::dvec4& outSample,
                const Raycast& outRay);

        //virtual void gatherLightHitsToward(
        //        std::vector<Raycast>& outRaycasts,
        //       const glm::dvec3& targetPos);


        virtual glm::dvec3 draft(const RayHitReport& report);

        void commitSample(const glm::dvec4& sample);


    private:
        std::atomic<bool> _runningPredicate;
        std::atomic<bool> _terminatePredicate;
        std::condition_variable _cv;
        std::mutex _flowMutex;

        std::atomic<bool> _incomingTileOnly;
        std::atomic<bool> _useStochasticTracing;
        std::atomic<bool> _usePixelJittering;
        std::atomic<bool> _useDepthOfField;

        double _lightRayIntensityThreshold;
        unsigned int _lightDirectRayCount;
        unsigned int _lightFireRayCount;
        unsigned int _maxScreenBounceCount;
        double _screenRayIntensityThreshold;
        unsigned int _sufficientScreenRayBounce;
        double _sufficientScreenRayWeight;
        double _minScreenRayWeight;

        glm::ivec2 _resolution;
        glm::dmat4 _viewInvMatrix;
        glm::dmat4 _projInvMatrix;
        glm::dmat4 _viewProjInverse;
        glm::dvec3 _camPos;
        glm::dvec3 _camDir;
        double _aperture;
        double _confusionRadius;
        glm::dvec3 _confusionSide;
        glm::dvec3 _confusionUp;

        glm::dvec4 _workingSample;
        std::shared_ptr<Film> _workingFilm;

        std::shared_ptr<StageSet> _stageSet;
        std::shared_ptr<Backdrop> _backdrop;
        std::shared_ptr<Material> _ambMaterial;
        std::shared_ptr<SearchStructure> _searchStructure;

        //std::vector<RayHitReport> _lightHitReports;

        // Memory pools
        RayHitList _rayHitList;
        std::vector<LightCast> _lightRays;
        std::vector<Raycast> _rayBounceArray;
        std::vector<Raycast> _tempChildRayArray;

        // Random distribution
        cellar::LinearRand _linearRand;
        cellar::DiskRand _diskRand;
    };
}

#endif // PROPROOM3D_CPURAYTRACERWORKER_H
