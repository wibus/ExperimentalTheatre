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
    class Material;
    class Coating;
    class Surface;
    class Prop;

    class AbstractTeam;

    class AbstractFilm;

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
        virtual void updateViewport(
                const glm::ivec2& resolution,
                const glm::ivec2& origin,
                const glm::ivec2& size);


        // TODO
        virtual void useStochasticTracing(bool use);
        virtual void usePixelJittering(bool use);
        virtual void useDepthOfField(bool use);

        virtual size_t completedFrameCount();
        virtual std::shared_ptr<AbstractFilm> readNextFilm();
        virtual void popReadFilm();

    protected:
        virtual void skipAndExecute(const std::function<void()>& func);

        virtual void execute();
        //virtual void shootFromLights();
        virtual void shootFromScreen();

        //virtual void fireLightRay(
        //        const Raycast& fromLightRay);

        virtual glm::dvec4 fireScreenRay(
                const Raycast& fromEyeRay);

        //virtual glm::dvec3 gatherScatteredLight(
        //        const Material& material,
        //        const Raycast& outRay);

        //virtual glm::dvec3 gatherReflectedLight(
        //        const Coating& coating,
        //        const Material& material,
        //        const RayHitReport& hitReport);

        //virtual void gatherLightHitsToward(
        //        std::vector<Raycast>& outRaycasts,
        //       const glm::dvec3& targetPos);


        virtual void compileSearchStructures();

        virtual double findNearestProp(
                const Raycast& raycast,
                RayHitReport& reportMin);

        virtual glm::dvec3 draft(
                const Raycast& raycast,
                const RayHitReport& report);

    private:
        void resetBuffers();
        void destroyBuffers();
        void getNewWorkingBuffers();
        void commitWorkingBuffers();


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
        double _screenRayIntensityThreshold;
        unsigned int _lightDirectRayCount;
        unsigned int _lightFireRayCount;

        glm::ivec2 _resolution;
        glm::ivec2 _viewportOrig;
        glm::ivec2 _viewportSize;
        glm::dmat4 _viewInvMatrix;
        glm::dmat4 _projInvMatrix;
        glm::dmat4 _viewProjInverse;
        glm::dvec3 _camPos;
        double _confusionRadius;

        std::queue<std::shared_ptr<AbstractFilm>> _completedFilms;
        std::vector<std::shared_ptr<AbstractFilm>> _filmPool;
        std::shared_ptr<AbstractFilm> _workingFilm;
        std::mutex _framesMutex;

        std::string _stageSetStream;
        std::shared_ptr<AbstractTeam> _team;
        std::shared_ptr<Backdrop> _backdrop;
        std::shared_ptr<Material> _envMaterial;
        std::shared_ptr<StageSet> _stageSet;

        std::vector<RayHitReport> _lightHitReports;
        std::vector<SearchZone> _searchZones;
        std::vector<std::shared_ptr<Surface>> _searchSurfaces;

        // Memory pools
        RayHitList _rayHitList;
    };
}

#endif // PROPROOM3D_CPURAYTRACERWORKER_H
