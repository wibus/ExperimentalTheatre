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
    class Ray;
    class Prop;

    class PROP3D_EXPORT CpuRaytracerWorker
    {
    public:
        static void launchWorker(
            const std::shared_ptr<CpuRaytracerWorker>& worker);

        CpuRaytracerWorker();
        virtual ~CpuRaytracerWorker();

        virtual void start();
        virtual void stop();
        virtual void terminate();

        virtual void resize(int width, int height);
        virtual void updateView(const glm::dmat4& view);
        virtual void updateProjection(const glm::dmat4& proj);
        virtual void setProps(const std::vector<std::shared_ptr<Prop>>& props);

        virtual unsigned int completedFrameCount();
        virtual const float* readNextFrame();
        virtual void popReadFrame();

    protected:
        virtual void skipAndExecute(const std::function<void()>& func);

        virtual void execute();
        virtual void shootFromLights();
        virtual void shootFromScreen();
        virtual glm::dvec3 fireScreenRay(
                const Ray& ray,
                int depth);

    private:
        void resetBuffers();
        void destroyBuffers();
        void getNewWorkingBuffers();
        void commitWorkingBuffers();


    private:
        std::atomic<bool> _runningPredicate;
        std::atomic<bool> _terminatePredicate;
        std::condition_variable _cv;
        std::mutex _flowMutex;

        int _lightRaysBounceCount;
        int _screenRaysBounceCount;

        glm::ivec2 _viewportSize;
        glm::dmat4 _viewInvMatrix;
        glm::dmat4 _projInvMatrix;
        glm::dmat4 _viewProjInverse;
        glm::dvec3 _camPos;

        std::queue<float*> _completedColorBuffers;
        std::vector<float*> _framePool;
        float* _workingColorBuffer;
        std::mutex _framesMutex;

        std::vector<std::shared_ptr<Prop>> _props;
    };
}

#endif // PROPROOM3D_CPURAYTRACERWORKER_H
