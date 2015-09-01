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


namespace prop3
{
    class Raycast;
    class RayHitReport;
    class Backdrop;
    class Material;
    class Prop;

    class AbstractTeam;


    class PROP3D_EXPORT CpuRaytracerWorker
    {
    public:
        static void launchWorker(
            const std::shared_ptr<CpuRaytracerWorker>& worker);

        CpuRaytracerWorker();
        virtual ~CpuRaytracerWorker();

        virtual void start(bool singleShot = false);
        virtual void stop();
        virtual void terminate();
        virtual bool isRunning();

        virtual void updateViewport(
                const glm::ivec2& resolution,
                const glm::ivec2& origin,
                const glm::ivec2& size);
        virtual void updateView(const glm::dmat4& view);
        virtual void updateProjection(const glm::dmat4& proj);

        virtual void setStageSetStream(const std::string& stream);

        // TODO
        virtual void useStochasticTracing(bool use);
        virtual void usePixelJittering(bool use);

        virtual unsigned int completedFrameCount();
        virtual const float* readNextFrame();
        virtual void popReadFrame();

    protected:
        virtual void skipAndExecute(const std::function<void()>& func);

        virtual void execute();
        virtual void shootFromLights();
        virtual void shootFromScreen();

        virtual std::shared_ptr<Material> findAmbientMaterial(
                glm::dvec3 position);

        virtual double findNearestProp(
                const Raycast& rayPrototype,
                prop3::RayHitReport& reportMin);

        virtual glm::dvec3 fireScreenRay(
                const Raycast& rayPrototype);

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

        std::atomic<bool> _usePixelJittering;
        std::atomic<bool> _useStochasticTracing;

        double _lightRayIntensityThreshold;
        double _screenRayIntensityThreshold;
        unsigned int _backdropRayCount;
        unsigned int _diffuseRayCount;

        glm::ivec2 _resolution;
        glm::ivec2 _viewportOrig;
        glm::ivec2 _viewportSize;
        glm::dmat4 _viewInvMatrix;
        glm::dmat4 _projInvMatrix;
        glm::dmat4 _viewProjInverse;
        glm::dvec3 _camPos;
        double _confusionRadius;

        std::queue<float*> _completedColorBuffers;
        std::vector<float*> _framePool;
        float* _workingColorBuffer;
        std::mutex _framesMutex;

        std::string _stageSetStream;
        std::shared_ptr<AbstractTeam> _team;
        std::shared_ptr<Backdrop> _backdrop;
        std::shared_ptr<Material> _envMaterial;
        std::vector<std::shared_ptr<Prop>> _props;

        std::vector<glm::dvec3> _lightHitColors;
        std::vector<RayHitReport> _lightHitReports;

        // Memory pools
        std::vector<RayHitReport*> _reportPool;
    };
}

#endif // PROPROOM3D_CPURAYTRACERWORKER_H
