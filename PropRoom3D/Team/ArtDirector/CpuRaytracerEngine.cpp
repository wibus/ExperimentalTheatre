#include "CpuRaytracerEngine.h"

#include <list>
#include <iostream>
#include <algorithm>

#include <CellarWorkbench/Misc/Log.h>

#include "Prop/Prop.h"
#include "Node/StageSet.h"
#include "Serial/JsonWriter.h"
#include "Film/StaticFilm.h"
#include "Film/ConvergentFilm.h"
#include "CpuRaytracerWorker.h"
#include "RaytracerState.h"


namespace prop3
{
    const unsigned int CpuRaytracerEngine::DEFAULT_WORKER_COUNT = 4;

    CpuRaytracerEngine::CpuRaytracerEngine() :
        _protectedState(),
        _raytracerState(new RaytracerState(_protectedState)),
        _isUpdated(false),
        _viewportSize(1, 1)
    {
        // hardware_concurrency is only a hint on the number of cores
        _protectedState.setWorkerCount(
            std::thread::hardware_concurrency());

        if(_raytracerState->workerCount() < 1)
        {
            _protectedState.setWorkerCount( DEFAULT_WORKER_COUNT );
        }
    }

    CpuRaytracerEngine::CpuRaytracerEngine(unsigned int  workerCount) :
        _protectedState(),
        _raytracerState(new RaytracerState(_protectedState)),
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

    void CpuRaytracerEngine::setup(
            double divergenceThreshold,
            const RaytracerState::DraftParams& draftParams)
    {
        _protectedState.setDivergenceThreshold(divergenceThreshold);
        _protectedState.setDraftParams(draftParams);

        setupWorkers();
        setupFilms();
    }

    void CpuRaytracerEngine::reset()
    {
        for(auto& w : _workerObjects)
        {
            w->terminate();
        }

        for(std::thread& t : _workerThreads)
        {
            t.join();
        }

        _workerThreads.clear();
    }

    void CpuRaytracerEngine::update(const std::shared_ptr<StageSet>& stageSet)
    {
        if(stageSet->stageSetChanged())
        {
            abortRendering();
            dispatchStageSet(stageSet);
        }

        if(stageSet->props().empty())
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
                _protectedState.incSampleCount();
                auto film = w->readNextFilm();
                incorporateFilm(*film);
                w->popReadFilm();
            }
        }
    }

    void CpuRaytracerEngine::pourFramesIn(
            const AbstractFilm& film)
    {
        skipDrafting();

        incorporateFilm(film);
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
            if(_raytracerState->converged())
            {
                interruptWorkers();
            }
        }
    }

    const AbstractFilm& CpuRaytracerEngine::film() const
    {
        return *_currentFilm;
    }

    std::shared_ptr<RaytracerState> CpuRaytracerEngine::raytracerState() const
    {
        return _raytracerState;
    }

    void CpuRaytracerEngine::resize(int width, int height)
    {
        // Stop workers
        interruptWorkers();

        // Reset buffers
        hardReset();


        // Draft films: intermediate resolution shots
        _viewportSize = glm::ivec2(width, height);
        int levelCount = _raytracerState->draftLevelCount();
        for(int i=0; i < levelCount; ++i)
        {
            int ratioPower = (_raytracerState->draftLevelCount() - (i+1));
            int ratio = _raytracerState->draftLevelSizeRatio()
                            * glm::pow(2, ratioPower);

            glm::ivec2 resolution = _viewportSize / glm::ivec2(ratio);
            resolution = glm::max(resolution, glm::ivec2(1, 1));

            _films[i]->resize(resolution);
        }


        // Main film: the one that contains the full res shot
        if(!_films.empty())
        {
            _films.back()->resize(_viewportSize);
        }


        if(!_raytracerState->isDrafter())
        {
            for(auto& w : _workerObjects)
            {
                w->updateViewport(
                    _viewportSize,
                    glm::ivec2(0, 0),
                    _viewportSize);
            }
        }
        else
        {
            _protectedState.setDraftLevel(-1);
            nextDraftSize();
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

    void CpuRaytracerEngine::dispatchStageSet(const std::shared_ptr<StageSet>& stageSet)
    {
        StageSetJsonWriter writer;
        std::string stageSetStream = writer.serialize(*stageSet);

        for(auto& w : _workerObjects)
        {
            w->updateStageSet(stageSetStream);
        }
    }

    void CpuRaytracerEngine::abortRendering()
    {
        // Stop workers
        interruptWorkers();

        // Reset buffers
        hardReset();

        // Reset draft state
        if(_raytracerState->isDrafter())
        {
            _protectedState.setDraftLevel( -1 );
            nextDraftSize();
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
        _protectedState.setDivergence( 1.0 );
        _protectedState.resetSampleCount();

        _currentFilm = _films[_raytracerState->draftLevel()];

        if(!_raytracerState->isDrafting())
        {
            // Drafting's finished
            _protectedState.startTimeChrono();
        }

        // Update worker buffers' size
        for(auto& w : _workerObjects)
        {
            w->updateViewport(
                _currentFilm->resolution(),
                glm::ivec2(0, 0),
                _currentFilm->resolution());
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

    void CpuRaytracerEngine::setupWorkers()
    {
        if(!_workerThreads.empty())
        {
            cellar::getLog().postMessage(new cellar::Message('I', false,
                "Terminating current CPU raytracer workers",
                "CpuRaytracerEngine"));

            reset();
        }


        size_t workerCount = _raytracerState->workerCount();

        cellar::getLog().postMessage(new cellar::Message('I', false,
            "Using " + std::to_string(workerCount) + " raytracer workers to render stageSet",
            "CpuRaytracerEngine"));

        for(size_t i=0; i < workerCount; ++i)
        {
            std::shared_ptr<CpuRaytracerWorker> worker(
                new CpuRaytracerWorker());

            _workerObjects.push_back(worker);
            _workerThreads.push_back(
                std::move(std::thread(
                    CpuRaytracerWorker::launchWorker,
                    _workerObjects[i])));
        }
    }

    void CpuRaytracerEngine::setupFilms()
    {
        if(!_films.empty())
        {
            cellar::getLog().postMessage(new cellar::Message('I', false,
                "Clearing current raytracer engine films",
                "CpuRaytracerEngine"));

            _films.clear();
        }

        if(_raytracerState->isDrafter())
        {
            // Drafts: non-stocastic and intermediate resolution shots
            int draftLevelCount = _raytracerState->draftLevelCount();
            for(int i=0; i < draftLevelCount; ++i)
            {
                _films.push_back(std::shared_ptr<AbstractFilm>(new StaticFilm()));
            }
        }
        size_t drafCount = _films.size();

        // Main film: full resolution shot
        _films.push_back(std::shared_ptr<AbstractFilm>(new ConvergentFilm()));

        cellar::getLog().postMessage(new cellar::Message('I', false,
            "Scene will be raytraced with " + std::to_string(drafCount) + " drafts",
            "CpuRaytracerEngine"));

        bool fastDraft = _raytracerState->isDrafter() && _raytracerState->fastDraftEnabled();
        cellar::getLog().postMessage(new cellar::Message('I', false,
            "Fast draft " + std::string(fastDraft ? "enabled" : "disabled"),
            "CpuRaytracerEngine"));

        resize(_viewportSize.x, _viewportSize.y);
    }

    void CpuRaytracerEngine::hardReset()
    {
        _protectedState.resetSampleCount();
        for(auto& film : _films)
        {
            film->clear();
        }

        if(!_films.empty())
        {
            _currentFilm = _films.front();
        }
    }

    void CpuRaytracerEngine::incorporateFilm(const AbstractFilm& film)
    {
        _currentFilm->mergeFilm(film);

        _protectedState.setDivergence(_currentFilm->compileDivergence());

        _isUpdated = true;
    }

    void CpuRaytracerEngine::performNonStochasticSyncronousDraf()
    {
        typedef decltype(_workerObjects.front()) WorkerPtr;
        typedef std::pair<int, WorkerPtr> DrafterType;
        std::list<DrafterType> drafters;

        int currOriginY = 0;
        int workerIdx = 0;
        int workerCount = (int)_workerObjects.size();
        glm::ivec2 viewport = _currentFilm->resolution();
        for(auto& w : _workerObjects)
        {
            glm::ivec2 orig = glm::ivec2(0, currOriginY);
            int nextOriginY = (viewport.y * (++workerIdx)) / workerCount;
            int viewportHeight = nextOriginY - currOriginY;
            glm::ivec2 size(viewport.x, viewportHeight);

            w->updateViewport(viewport, orig, size);
            w->useStochasticTracing(false);
            w->usePixelJittering(false);
            w->useDepthOfField(false);
            w->start(true);

            drafters.push_back(DrafterType(currOriginY, w));
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
                    int originY = it->first;
                    glm::ivec2 filmOrigin(0, originY);

                    std::shared_ptr<AbstractFilm> film = worker->readNextFilm();
                    _currentFilm->blitFilm(filmOrigin, *film);

                    worker->popReadFilm();
                    worker->useStochasticTracing(true);
                    worker->usePixelJittering(true);
                    worker->useDepthOfField(true);
                    it = drafters.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        // Set this draft level as complete
        _protectedState.addSampleCount(
            _raytracerState->draftFrameCountPerLevel() * workerCount );

        // Notify user that a new frame is ready
        _isUpdated = true;
    }
}
