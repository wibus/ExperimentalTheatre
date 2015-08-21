#ifndef PROPROOM3D_CPURAYTRACERENGINE_H
#define PROPROOM3D_CPURAYTRACERENGINE_H

#include <vector>
#include <thread>

#include <GLM/glm.hpp>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    class Prop;
    class Scene;
    class CpuRaytracerWorker;


    class PROP3D_EXPORT CpuRaytracerEngine
    {
    public:
        CpuRaytracerEngine();
        CpuRaytracerEngine(unsigned int workerCount);
        virtual ~CpuRaytracerEngine();

        virtual void setup(const std::shared_ptr<Scene>& scene);
        virtual void reset();

        virtual bool isDrafter() const;
        virtual bool isDrafting() const;
        virtual void setDraftParams(
                int levelCount,
                int levelSizeRatio,
                int threadBatchPerLevel);
        virtual void enableFastDraft(bool enable);

        virtual bool isUpdated();
        virtual bool onUpdateConsumed();
        virtual float renderTime() const;
        virtual float divergenceValue() const;
        virtual float imageVariance() const;
        virtual unsigned int sampleCount() const;
        virtual const glm::ivec2& viewportSize() const;
        virtual const std::vector<float>& colorBuffer() const;

        virtual void update();
        virtual void pourFramesIn(
            const std::vector<float>& colorBuffer,
            unsigned int sampleCount);
        virtual void pourFramesOut(
            std::vector<float>& colorBuffer,
            unsigned int& sampleCount);

        virtual void resize(int width, int height);
        virtual void updateView(const glm::dmat4& view);
        virtual void updateProjection(const glm::dmat4& proj);


    protected:
        virtual void dispatchScene();
        virtual void skipDrafting();
        virtual void nextDraftSize();
        virtual void abortRendering();
        virtual void bufferSoftReset();
        virtual void bufferHardReset();
        virtual void incorporateFrames(
            const float* colorBuffer,
            unsigned int sampleCount);

        virtual void performNonStochasticSyncronousDraf();

    private:
        static const unsigned int DEFAULT_WORKER_COUNT;

        float _divergenceValue;
        float _imageVariance;
        unsigned int _sampleCount;
        std::chrono::steady_clock::time_point _startTime;

        int _draftLevel;
        int _draftLevelCount;
        int _draftLevelSizeRatio;
        int _draftThreadBatchPerLevel;
        glm::ivec2 _draftViewportSize;
        bool _fastDraftEnabled;
        bool _fastDraftDone;

        bool _isUpdated;
        glm::ivec2 _viewportSize;
        std::vector<float> _colorBuffer;
        std::shared_ptr<Scene> _scene;

        friend class CpuRaytracerWorker;
        bool _workersInterrupted;
        std::vector<std::thread> _workerThreads;
        std::vector<std::shared_ptr<CpuRaytracerWorker>> _workerObjects;
    };
}

#endif // PROPROOM3D_CPURAYTRACERENGINE_H
