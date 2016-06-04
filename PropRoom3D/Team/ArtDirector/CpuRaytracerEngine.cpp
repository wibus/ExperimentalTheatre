#include "CpuRaytracerEngine.h"

#include <list>
#include <iostream>
#include <algorithm>

#include <CellarWorkbench/Misc/Log.h>

#include "Node/Prop/Prop.h"
#include "Node/StageSet.h"
#include "Film/StaticFilm.h"
#include "Film/ConvergentFilm.h"
#include "CpuRaytracerWorker.h"
#include "RaytracerState.h"
#include "SearchStructure.h"


namespace prop3
{
    const unsigned int CpuRaytracerEngine::DEFAULT_WORKER_COUNT = 4;

    CpuRaytracerEngine::CpuRaytracerEngine() :
        _protectedState(),
        _raytracerState(new RaytracerState(_protectedState)),
        _viewportSize(1, 1),
        _cameraChanged(false),
        _stageSetUpdated(false)
    {
        // hardware_concurrency is only a hint on the number of cores
        _protectedState.setWorkerCount(
            std::thread::hardware_concurrency());

        if(_raytracerState->workerCount() < 1)
        {
            _protectedState.setWorkerCount( DEFAULT_WORKER_COUNT );
        }

        cellar::g_masterRandomArray.allocate();
    }

    CpuRaytracerEngine::CpuRaytracerEngine(unsigned int  workerCount) :
        _protectedState(),
        _raytracerState(new RaytracerState(_protectedState)),
        _viewportSize(1, 1),
        _cameraChanged(false),
        _stageSetUpdated(false)
    {
        _protectedState.setWorkerCount( workerCount );

        cellar::g_masterRandomArray.allocate();
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

        cellar::g_masterRandomArray.deallocate();
    }

    void CpuRaytracerEngine::setup(
            const RaytracerState::DraftParams& draftParams,
            const std::shared_ptr<Film>& mainFilm)
    {
        _protectedState.setDraftParams(draftParams);

        setupFilms(mainFilm);
        setupWorkers();
    }

    void CpuRaytracerEngine::terminate()
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

    void CpuRaytracerEngine::update()
    {
        if(_stageSetUpdated || _cameraChanged)
        {
            abortRendering();

            bool hiddenSurfaceRemoved = _raytracerState->hiddenSurfacesRemoved();
            bool debuggingSurfRemoval = _raytracerState->debuggingHiddenSurfaceRemoval();

            if(_stageSetUpdated || (hiddenSurfaceRemoved && !debuggingSurfRemoval))
                dispatchStageSet(_stageSetStream);
            else if(_searchStructure.get() != nullptr)
                _searchStructure->resetHitCounters();

            _cameraChanged = false;
            _stageSetUpdated = false;
        }

        if(_stageSetStream.empty())
            return;

        if(!_raytracerState->isRendering())
        {
            if(_currentFilm->incomingTileAvailable())
            {
                for(auto& w : _workerObjects)
                    if(!w->isRunning())
                        w->start(true);
            }

            return;
        }

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
        else
        {
            for(auto& w : _workerObjects)
            {
                if(!w->isRunning())
                {
                    w->start();
                }
            }

            return;
        }
    }

    void CpuRaytracerEngine::interrupt()
    {
        interruptWorkers(true);
    }

    void CpuRaytracerEngine::manageNextFrame()
    {
        _protectedState.incSampleCount();
        _protectedState.setDivergence(
            _currentFilm->compileDivergence());

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
            if(_raytracerState->sampleCount() == 1)
            {
                optimizeSearchStructure();
            }

            if(_raytracerState->sampleCount() > 2 && (
               _raytracerState->converged()) ||
               _raytracerState->runningOutOfTime() ||
               _raytracerState->runningOutOfSamples())
            {
                interruptWorkers();

                if(_raytracerState->converged())
                    _currentFilm->backupAsReferenceShot();
            }
        }
    }

    bool CpuRaytracerEngine::newTileCompleted()
    {
        return _currentFilm->newTileCompleted();
    }

    bool CpuRaytracerEngine::newFrameCompleted()
    {
        return _currentFilm->newFrameCompleted();
    }

    std::shared_ptr<Film> CpuRaytracerEngine::currentFilm() const
    {
        return _currentFilm;
    }

    std::shared_ptr<RaytracerState> CpuRaytracerEngine::raytracerState() const
    {
        return _raytracerState;
    }

    void CpuRaytracerEngine::resize(int width, int height)
    {
        // Stop workers
        interruptWorkers(true);

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

            _films[i]->resizeFrame(resolution);
        }


        // Main film: the one that contains the full res shot
        if(!_films.empty())
        {
            _films.back()->resizeFrame(_viewportSize);
        }

        if(_raytracerState->isDrafter())
        {
            _protectedState.setDraftLevel(-1);
            nextDraftSize();
        }
    }

    void CpuRaytracerEngine::updateView(const glm::dmat4& view)
    {
        _cameraChanged = true;

        for(auto& w : _workerObjects)
        {
            w->updateView(view);
        }
    }

    void CpuRaytracerEngine::updateProjection(const glm::dmat4& proj)
    {
        _cameraChanged = true;

        for(auto& w : _workerObjects)
        {
            w->updateProjection(proj);
        }
    }

    void CpuRaytracerEngine::updateStageSet(const std::string& stageSet)
    {
        _stageSetUpdated = true;
        _stageSetStream = stageSet;
    }

    void CpuRaytracerEngine::interruptWorkers(bool wait)
    {
        _protectedState.setInterrupted( true );
        for(auto& w : _workerObjects)
            w->stop();

        if(wait)
        {
            for(auto& w : _workerObjects)
                w->waitForStop();
        }
    }

    void CpuRaytracerEngine::dispatchStageSet(const std::string& stageSet)
    {
        interruptWorkers(true);

        _searchStructure.reset(new SearchStructure(stageSet));
        _protectedState.setHiddenSurfaceRemoved(false);

        for(auto& w : _workerObjects)
        {
            w->updateSearchStructure(_searchStructure);
        }
    }

    void CpuRaytracerEngine::setupFilms(const std::shared_ptr<Film>& mainFilm)
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

            // Non-Stochastic
            if(_raytracerState->fastDraftEnabled())
            {
                _films.push_back(std::shared_ptr<Film>(new StaticFilm()));
                --draftLevelCount;
            }

            // Intermediates
            for(int i=0; i < draftLevelCount; ++i)
            {
                _films.push_back(std::shared_ptr<Film>(new ConvergentFilm()));
            }
        }
        size_t drafCount = _films.size();

        // Main film: full resolution shot
        _films.push_back(mainFilm);

        _currentFilm = _films.front();


        cellar::getLog().postMessage(new cellar::Message('I', false,
            "Scene will be raytraced with " + std::to_string(drafCount) + " drafts",
            "CpuRaytracerEngine"));

        bool fastDraft = _raytracerState->isDrafter() && _raytracerState->fastDraftEnabled();
        cellar::getLog().postMessage(new cellar::Message('I', false,
            "Fast draft " + std::string(fastDraft ? "enabled" : "disabled"),
            "CpuRaytracerEngine"));

        resize(_viewportSize.x, _viewportSize.y);
    }

    void CpuRaytracerEngine::optimizeSearchStructure()
    {
        interruptWorkers(true);

        size_t removedZones;
        size_t removedSurfaces;

        int surfVisToPixCount = _raytracerState->surfaceVisibilityThreshold()
                * (_currentFilm->frameWidth() * _currentFilm->frameHeight());

        _searchStructure->removeHiddenSurfaces(
                surfVisToPixCount,
                removedZones,
                removedSurfaces);

        _protectedState.setHiddenSurfaceRemoved(true);

        cellar::getLog().postMessage(new cellar::Message('I', false,
            "Hidden surface removed : "
            + std::to_string(removedZones) + "z, "
            + std::to_string(removedSurfaces) + "s",
            "CpuRaytracerEngine"));
    }

    void CpuRaytracerEngine::abortRendering()
    {
        // Stop workers
        interruptWorkers(true);

        // Reset buffers
        softReset();

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
            w->updateFilm(_currentFilm);
        }
    }

    void CpuRaytracerEngine::setupWorkers()
    {
        if(!_workerThreads.empty())
        {
            cellar::getLog().postMessage(new cellar::Message('I', false,
                "Terminating current CPU raytracer workers",
                "CpuRaytracerEngine"));

            terminate();
        }


        size_t workerCount = _raytracerState->workerCount();

        cellar::getLog().postMessage(new cellar::Message('I', false,
            "Using " + std::to_string(workerCount) + " raytracer workers to render stageSet",
            "CpuRaytracerEngine"));

        for(size_t i=0; i < workerCount; ++i)
        {
            std::shared_ptr<CpuRaytracerWorker> worker(
                new CpuRaytracerWorker());

            worker->updateFilm(_currentFilm);

            _workerObjects.push_back(worker);
            _workerThreads.push_back(
                std::move(std::thread(
                    CpuRaytracerWorker::launchWorker,
                    _workerObjects[i])));
        }
    }

    void CpuRaytracerEngine::softReset()
    {
        _protectedState.resetSampleCount();

        // Manage drafting films
        for(size_t f=0; f < _films.size()-1; ++f)
        {
            _films[f]->clear();
        }

        if(!_films.empty())
        {
            // Manage Main Film
            if(_raytracerState->filmRawFilePath() !=
               RaytracerState::UNSPECIFIED_RAW_FILE)
            {
                if(!_films.back()->loadRawFilm(
                    _raytracerState->filmRawFilePath()))
                {
                    _raytracerState->setFilmRawFilePath(
                        RaytracerState::UNSPECIFIED_RAW_FILE);
                    _films.back()->clear();
                }
            }
            else
            {
                _films.back()->clear();
            }


            _currentFilm = _films.front();
        }
    }

    void CpuRaytracerEngine::performNonStochasticSyncronousDraf()
    {
        interruptWorkers(true);

        for(auto& w : _workerObjects)
        {
            w->updateFilm(_currentFilm);
            w->useStochasticTracing(false);
            w->usePixelJittering(false);
            w->useDepthOfField(false);
            w->start();
        }

        _currentFilm->waitForFrameCompletion();

        for(auto& w : _workerObjects)
        {
            w->useStochasticTracing(true);
            w->usePixelJittering(true);
            w->useDepthOfField(true);
        }

        // Set this draft level as complete
        _protectedState.addSampleCount(
            _raytracerState->draftFrameCountPerLevel() *
            _raytracerState->workerCount());
    }
}
