#include "CpuRaytracerEngine.h"

#include <list>
#include <iostream>
#include <algorithm>

#include <CellarWorkbench/Misc/Log.h>

#include "Prop/Prop.h"
#include "Node/StageSet.h"
#include "Serial/JsonWriter.h"
#include "CpuRaytracerWorker.h"
#include "RaytracerState.h"


namespace prop3
{
    const unsigned int CpuRaytracerEngine::DEFAULT_WORKER_COUNT = 4;

    CpuRaytracerEngine::CpuRaytracerEngine() :
        _protectedState(),
        _raytracerState(new RaytracerState(_protectedState)),
        _draftViewportSize(1, 1),
        _isUpdated(false),
        _viewportSize(1, 1)
    {
        // hardware_concurrency is only a hint on the number of cores
        _protectedState.setWorkerCount( //1 + 0 *
            std::thread::hardware_concurrency());

        if(_raytracerState->workerCount() < 1)
        {
            _protectedState.setWorkerCount( DEFAULT_WORKER_COUNT );
        }
    }

    CpuRaytracerEngine::CpuRaytracerEngine(unsigned int  workerCount) :
        _protectedState(),
        _raytracerState(new RaytracerState(_protectedState)),
        _draftViewportSize(1, 1),
        _isUpdated(false),
        _viewportSize(1, 1)
    {
        _protectedState.setWorkerCount( workerCount );
    }

    CpuRaytracerEngine::~CpuRaytracerEngine()
    {
        for(auto& w : _workerObjects)
        {
            w->terminate();
        }

        for(std::thread& t : _workerThreads)
        {
            t.join();
        }
    }

    void CpuRaytracerEngine::setup(const std::shared_ptr<StageSet>& stageSet)
    {
        _stageSet = stageSet;

        size_t workerCount = _raytracerState->workerCount();

        cellar::getLog().postMessage(new cellar::Message('I', false,
            "Using " + std::to_string(workerCount) + " raytracer workers to render stageSet",
            "CpuRaytracer"));

        for(size_t i=0; i < workerCount; ++i)
        {
            std::shared_ptr<CpuRaytracerWorker> worker(
                new CpuRaytracerWorker());

            _workerObjects.push_back(worker);
            _workerThreads.push_back(std::move(std::thread(
                CpuRaytracerWorker::launchWorker,
                _workerObjects[i])));
        }

        bufferHardReset();
    }

    void CpuRaytracerEngine::reset()
    {
        abortRendering();
        dispatchStageSet();
    }

    void CpuRaytracerEngine::update()
    {
        if(_stageSet->stageSetChanged())
        {
            abortRendering();
            dispatchStageSet();
        }

        if(_stageSet->props().empty())
            return;

        if(!_raytracerState->isRendering())
            return;

        if(_raytracerState->interrupted())
        {
            _protectedState.setInterrupted( false );

            if(_raytracerState->isDrafter())
            {
                if(_raytracerState->draftLevel() == 0 &&
                   _raytracerState->fastDraftEnabled())
                {
                    performNonStochasticSyncronousDraf();
                }
            }
            else
            {
                _protectedState.startTimeChrono();
            }

            for(auto& w : _workerObjects)
            {
                if(!w->isRunning())
                {
                    w->start();
                }
            }

            return;
        }


        for(auto& w : _workerObjects)
        {
            while(w->completedFrameCount() != 0)
            {
                const float* pixels = w->readNextFrame();
                incorporateFrames(pixels, 1);
                w->popReadFrame();
            }
        }
    }

    void CpuRaytracerEngine::pourFramesIn(
            const std::vector<float>& colorBuffer,
            unsigned int sampleCount)
    {
        skipDrafting();

        incorporateFrames(colorBuffer.data(), sampleCount);
    }

    void CpuRaytracerEngine::pourFramesOut(
            std::vector<float>& colorBuffer,
            unsigned int& sampleCount)
    {
        sampleCount = _raytracerState->sampleCount();
        _protectedState.setSampleCount( 0 );

        colorBuffer.resize(_colorBuffer.size());
        std::copy(_colorBuffer.begin(), _colorBuffer.end(), colorBuffer.begin());

        _isUpdated = false;
    }

    bool CpuRaytracerEngine::isUpdated()
    {
        return _isUpdated;
    }

    void CpuRaytracerEngine::onUpdateConsumed()
    {
        _isUpdated = false;

        if(_raytracerState->isDrafting())
        {
            if(_raytracerState->sampleCount() >=
               _raytracerState->draftFrameCountPerLevel()
                    * _workerThreads.size())
            {
                nextDraftSize();
            }
        }
        else
        {
            if(_raytracerState->divergence() <=
               _raytracerState->divergenceThreshold())
            {
                interruptWorkers();
            }
        }
    }

    const glm::ivec2& CpuRaytracerEngine::viewportSize() const
    {
        if(_raytracerState->isDrafting())
            return _draftViewportSize;
        return _viewportSize;
    }

    const std::vector<float>& CpuRaytracerEngine::colorBuffer() const
    {
        return _colorBuffer;
    }

    std::shared_ptr<RaytracerState> CpuRaytracerEngine::raytracerState() const
    {
        return _raytracerState;
    }

    void CpuRaytracerEngine::resize(int width, int height)
    {
        abortRendering();

        _viewportSize = glm::ivec2(width, height);
        _colorBuffer.resize(width * height * 3);

        if(!_raytracerState->isDrafting())
        {
            for(auto& w : _workerObjects)
            {
                w->updateViewport(
                    _viewportSize,
                    glm::ivec2(0, 0),
                    _viewportSize);
            }
        }
    }

    void CpuRaytracerEngine::updateView(const glm::dmat4& view)
    {
        abortRendering();

        for(auto& w : _workerObjects)
        {
            w->updateView(view);
        }
    }

    void CpuRaytracerEngine::updateProjection(const glm::dmat4& proj)
    {
        abortRendering();

        for(auto& w : _workerObjects)
        {
            w->updateProjection(proj);
        }
    }

    void CpuRaytracerEngine::dispatchStageSet()
    {
        StageSetJsonWriter writer;
        std::string stageSetStream = writer.serialize(*_stageSet);

        for(auto& w : _workerObjects)
        {
            w->setStageSetStream(stageSetStream);
        }
    }

    void CpuRaytracerEngine::skipDrafting()
    {
        if(!_raytracerState->isDrafting())
            return;

        _protectedState.setDraftLevel(
            _raytracerState->draftLevelCount() - 1);

        nextDraftSize();
    }

    void CpuRaytracerEngine::nextDraftSize()
    {
        if(!_raytracerState->isDrafting())
            return;

        _protectedState.setDraftLevel(
            _raytracerState->draftLevel() + 1);
        _protectedState.setSampleCount( 0 );
        _protectedState.setDivergence( 1.0 );

        if(_raytracerState->isDrafting())
        {
            // Drafting not finished
            int ratioPower = (raytracerState()->draftLevelCount()
                              - (raytracerState()->draftLevel() + 1));

            int ratio = glm::pow(2, ratioPower) *
                        raytracerState()->draftLevelSizeRatio();

            _draftViewportSize = _viewportSize / glm::ivec2(ratio);
            _draftViewportSize = glm::max(_draftViewportSize, glm::ivec2(1));
        }
        else
        {
            // Drafting finished
            _draftViewportSize = _viewportSize;
            _protectedState.startTimeChrono();
        }

        // Update worker buffers' size
        for(auto& w : _workerObjects)
        {
            w->updateViewport(
                _draftViewportSize,
                glm::ivec2(0, 0),
                _draftViewportSize);
        }
    }

    void CpuRaytracerEngine::abortRendering()
    {
        // Reset buffers
        bufferSoftReset();

        // Stop workers
        interruptWorkers();

        // Reset draft state
        if(_raytracerState->isDrafter())
        {
            _protectedState.setDraftLevel( -1 );
            nextDraftSize();
        }
    }

    void CpuRaytracerEngine::interruptWorkers()
    {
        _protectedState.setInterrupted( true );
        for(auto& w : _workerObjects)
        {
            w->stop();
        }
    }

    void CpuRaytracerEngine::bufferSoftReset()
    {
        _protectedState.setSampleCount( 0 );
    }

    void CpuRaytracerEngine::bufferHardReset()
    {
        _protectedState.setSampleCount( 0 );

        _colorBuffer.resize(_viewportSize.x * _viewportSize.y * 3);
        std::fill(_colorBuffer.begin(), _colorBuffer.end(), 0.0f);
    }

    void CpuRaytracerEngine::incorporateFrames(
        const float* colorBuffer,
        unsigned int sampleCount)
    {
        glm::vec3 imageMean(0);
        glm::vec3 lineVariance(0);
        glm::vec3 totalVariance(0);

        int lastSampleCount = _raytracerState->sampleCount();
        _protectedState.setSampleCount(lastSampleCount + sampleCount);
        int nextSampleCount = _raytracerState->sampleCount();

        const glm::ivec2& viewport = viewportSize();
        int cc = viewport.x * viewport.y * 3;
        double lineWidth = viewport.x * 3;
        double endLineAt = lineWidth - 3;
        for(int i=0; i < cc; i+=3)
        {
            glm::vec3 lastValue(
                _colorBuffer[i],
                _colorBuffer[i+1],
                _colorBuffer[i+2]);

            glm::vec3 newValue(
                colorBuffer[i]   * sampleCount,
                colorBuffer[i+1] * sampleCount,
                colorBuffer[i+2] * sampleCount);

            int oldSampleBatch = sampleCount * 8;
            int oldSampleStock = lastSampleCount;
            while(oldSampleBatch < oldSampleStock)
            {
                newValue += lastValue * (float) oldSampleBatch;
                oldSampleStock -= oldSampleBatch;
                oldSampleBatch *= 8;
            }
            if(oldSampleStock != 0)
            {
                newValue += lastValue * (float) oldSampleStock;
            }

            newValue /= (float) nextSampleCount;
            glm::vec3 meanShift = newValue - lastValue;
            lineVariance += meanShift * meanShift;

            if(i == endLineAt)
            {
                totalVariance += lineVariance / (float)viewport.x;
                lineVariance = glm::dvec3(0);
                endLineAt += lineWidth;
            }

            _colorBuffer[i] = newValue.r;
            _colorBuffer[i+1] = newValue.g;
            _colorBuffer[i+2] = newValue.b;

            imageMean += newValue;
        }

        totalVariance /= (float) viewport.y;
        _protectedState.setDivergence(
            glm::length(glm::sqrt(totalVariance)) * sampleCount);

        _isUpdated = true;

        //if(nextSampleCount == 8)
        //    exit(0);
    }

    void CpuRaytracerEngine::performNonStochasticSyncronousDraf()
    {
        typedef decltype(_workerObjects.front()) WorkerPtr;
        typedef std::pair<glm::ivec2, WorkerPtr> DrafterType;
        std::list<DrafterType> drafters;

        int currOriginY = 0;
        int workerIdx = 0;
        int workerCount = (int)_workerObjects.size();
        const glm::ivec2& viewport = viewportSize();
        for(auto& w : _workerObjects)
        {
            glm::ivec2 orig = glm::ivec2(0, currOriginY);
            int nextOriginY = (viewport.y * (++workerIdx)) / workerCount;
            int viewportHeight = nextOriginY - currOriginY;
            glm::ivec2 size(viewport.x, viewportHeight);

            w->usePixelJittering(false);
            w->useStochasticTracing(false);
            w->updateViewport(viewport, orig, size);
            w->start(true);

            glm::ivec2 viewtParam(currOriginY, viewportHeight);
            drafters.push_back(DrafterType(viewtParam, w));
            currOriginY = nextOriginY;
        }

        while(!drafters.empty())
        {
            auto it = drafters.begin();
            while(it != drafters.end())
            {
                WorkerPtr& worker = it->second;
                if(worker->completedFrameCount() != 0)
                {
                    const glm::ivec2 viewParams = it->first;
                    const float* pixels = worker->readNextFrame();
                    int baseIdx = viewParams.x * viewport.x * 3;
                    int topIdx = (viewParams.x + viewParams.y) * viewport.x * 3;

                    for(int i=baseIdx; i<topIdx; ++i)
                    {
                        _colorBuffer[i] = pixels[i - baseIdx];
                    }

                    worker->popReadFrame();
                    worker->usePixelJittering(true);
                    worker->useStochasticTracing(true);
                    it = drafters.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        // Set this draft level as complete
        _protectedState.setSampleCount(
            _raytracerState->draftFrameCountPerLevel() * workerCount );

        // Notify user that a new frame is ready
        _isUpdated = true;
    }
}
