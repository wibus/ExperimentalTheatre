#ifndef PROPROOM3D_CPURAYTRACERWORKER_H
#define PROPROOM3D_CPURAYTRACERWORKER_H

#include <memory>
#include <vector>
#include <queue>

#include <atomic>
#include <mutex>
#include <condition_variable>

#include <functional>

#include <GLM/glm.hpp>

#include "../../libPropRoom3D_global.h"

#include "Ray/RayHitList.h"


namespace prop3
{
    class Raycast;
    class RayHitReport;
    class StageSet;
    class Backdrop;
    class LightBulb;
    class Material;
    class Coating;
    class Surface;
    class Prop;

    class AbstractTeam;

    class Film;
    class Tile;

    struct SearchZone;


    class PROP3D_EXPORT CpuRaytracerWorker
    {
    public:
        static void launchWorker(
            const std::shared_ptr<CpuRaytracerWorker>& worker);

        CpuRaytracerWorker();
        virtual ~CpuRaytracerWorker();

        // States
        virtual void start(bool singleShot = false);
        virtual void stop();
        virtual void terminate();
        virtual bool isRunning();

        // Updates
        virtual void updateStageSet(const std::string& stream);
        virtual void updateView(const glm::dmat4& view);
        virtual void updateProjection(const glm::dmat4& proj);
        virtual void updateFilm(std::shared_ptr<Film>& film);


        // TODO
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

        virtual void fireScreenRay(
                const Raycast& fromEyeRay);

        //virtual glm::dvec3 gatherScatteredLight(
        //        const Material& material,
        //        const Raycast& outRay);

        virtual void gatherReflectedLight(
                const Coating& coating,
                const RayHitReport& hitReport,
                const Raycast& outRay);

        //virtual void gatherLightHitsToward(
        //        std::vector<Raycast>& outRaycasts,
        //       const glm::dvec3& targetPos);


        virtual void compileSearchStructures();

        virtual double findNearestIntersection(
                const Raycast& raycast,
                RayHitReport& reportMin);

        virtual bool intersectsScene(
                const Raycast& raycast);

        virtual glm::dvec3 draft(const RayHitReport& report);

        void commitSample(const glm::dvec4& sample);


    private:
        std::atomic<bool> _isSingleShot;
        std::atomic<bool> _runningPredicate;
        std::atomic<bool> _terminatePredicate;
        std::condition_variable _cv;
        std::mutex _flowMutex;

        std::atomic<bool> _useStochasticTracing;
        std::atomic<bool> _usePixelJittering;
        std::atomic<bool> _useDepthOfField;

        double _lightRayIntensityThreshold;
        unsigned int _lightDirectRayCount;
        unsigned int _lightFireRayCount;
        unsigned int _maxScreenBounceCount;
        double _minScreenRayWeight;

        glm::ivec2 _resolution;
        glm::dmat4 _viewInvMatrix;
        glm::dmat4 _projInvMatrix;
        glm::dmat4 _viewProjInverse;
        glm::dvec3 _camPos;
        double _confusionRadius;

        glm::dvec4 _workingSample;
        std::shared_ptr<Film> _workingFilm;

        std::string _stageSetStream;
        std::shared_ptr<AbstractTeam> _team;
        std::shared_ptr<Backdrop> _backdrop;
        std::shared_ptr<Material> _ambMaterial;
        std::shared_ptr<StageSet> _stageSet;

        //std::vector<RayHitReport> _lightHitReports;
        std::vector<SearchZone> _searchZones;
        std::vector<std::shared_ptr<const Surface>> _searchSurfaces;
        std::vector<std::shared_ptr<const LightBulb>> _searchLights;

        // Memory pools
        RayHitList _rayHitList;
        std::vector<Raycast> _rayBounceArray;
        std::vector<Raycast> _tempChildRayArray;
    };
}

#endif // PROPROOM3D_CPURAYTRACERWORKER_H
