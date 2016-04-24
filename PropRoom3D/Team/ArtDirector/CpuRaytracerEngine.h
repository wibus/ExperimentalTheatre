#ifndef PROPROOM3D_CPURAYTRACERENGINE_H
#define PROPROOM3D_CPURAYTRACERENGINE_H

#include <vector>
#include <thread>
#include <memory>

#include <GLM/glm.hpp>

#include "RaytracerState.h"

#include <PropRoom3D/Node/Node.h>


namespace prop3
{
    class Film;
    class StageSet;
    class CpuRaytracerWorker;
    class SearchStructure;


    class PROP3D_EXPORT CpuRaytracerEngine
    {
	private:
		CpuRaytracerEngine(CpuRaytracerEngine&) = delete;
		CpuRaytracerEngine& operator= (CpuRaytracerEngine&) = delete;

    public:
        CpuRaytracerEngine();
        CpuRaytracerEngine(unsigned int workerCount);
        virtual ~CpuRaytracerEngine();

        virtual void setup(const RaytracerState::DraftParams& draftParams,
                           const std::shared_ptr<Film>& mainFilm);

        virtual void terminate();

        virtual bool newTileCompleted();
        virtual bool newFrameCompleted();
        virtual void manageNextFrame();
        virtual std::shared_ptr<Film> currentFilm() const;

        std::shared_ptr<RaytracerState> raytracerState() const;

        virtual void update();
        virtual void pourFramesIn(const Film& currentFilm);

        virtual void resize(int width, int height);
        virtual void updateView(const glm::dmat4& view);
        virtual void updateProjection(const glm::dmat4& proj);
        virtual void updateStageSet(const std::string& stageSet);


    protected:
        virtual void interruptWorkers(bool wait = false);
        virtual void dispatchStageSet(const std::string& stageSet);
        virtual void setupFilms(const std::shared_ptr<Film>& mainFilm);
        virtual void optimizeSearchStructure();
        virtual void abortRendering();
        virtual void skipDrafting();
        virtual void nextDraftSize();
        virtual void setupWorkers();
        virtual void softReset();
        virtual void incorporateFilm(
                const Film& currentFilm);

        virtual void performNonStochasticSyncronousDraf();

    private:
        static const unsigned int DEFAULT_WORKER_COUNT;

        RaytracerState::DraftParams _draftParams;
        RaytracerState::ProtectedState _protectedState;
        std::shared_ptr<RaytracerState> _raytracerState;

        bool _cameraChanged;
        glm::ivec2 _viewportSize;
        std::vector<std::shared_ptr<Film>> _films;
        std::shared_ptr<Film> _currentFilm;

        friend class CpuRaytracerWorker;
        std::vector<std::thread> _workerThreads;
        std::vector<std::shared_ptr<CpuRaytracerWorker>> _workerObjects;

        bool _stageSetUpdated;
        std::string _stageSetStream;
        std::shared_ptr<SearchStructure> _searchStructure;
    };
}

#endif // PROPROOM3D_CPURAYTRACERENGINE_H
