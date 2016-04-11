#ifndef PROPROOM3D_CPURAYTRACERENGINE_H
#define PROPROOM3D_CPURAYTRACERENGINE_H

#include <vector>
#include <thread>
#include <memory>

#include <GLM/glm.hpp>

#include "RaytracerState.h"


namespace prop3
{
    class Prop;
    class StageSet;
    class Film;
    class CpuRaytracerWorker;


    class PROP3D_EXPORT CpuRaytracerEngine
    {
	private:
		CpuRaytracerEngine(CpuRaytracerEngine&) = delete;
		CpuRaytracerEngine& operator= (CpuRaytracerEngine&) = delete;

    public:
        CpuRaytracerEngine();
        CpuRaytracerEngine(unsigned int workerCount);
        virtual ~CpuRaytracerEngine();

        virtual void setup(const RaytracerState::DraftParams& draftParams);

        virtual void terminate();

        virtual bool newTileCompleted();
        virtual bool newFrameCompleted();
        virtual void manageNextFrame();
        virtual std::shared_ptr<Film> film() const;

        std::shared_ptr<RaytracerState> raytracerState() const;

        virtual void update(const std::shared_ptr<StageSet>& stageSet);
        virtual void pourFramesIn(const Film& film);

        virtual void resize(int width, int height);
        virtual void updateView(const glm::dmat4& view);
        virtual void updateProjection(const glm::dmat4& proj);


    protected:
        virtual void dispatchStageSet(const std::shared_ptr<StageSet>& stageSet);
        virtual void abortRendering();
        virtual void skipDrafting();
        virtual void nextDraftSize();
        virtual void interruptWorkers();
        virtual void setupWorkers();
        virtual void setupFilms();
        virtual void softReset();
        virtual void incorporateFilm(
                const Film& film);

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
    };
}

#endif // PROPROOM3D_CPURAYTRACERENGINE_H
