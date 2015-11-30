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
    class AbstractFilm;
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

        virtual void setup(double divergenceThreshold,
                           const RaytracerState::DraftParams& draftParams);
        virtual void reset();

        virtual bool isUpdated();
        virtual void onUpdateConsumed();
        virtual const AbstractFilm& film() const;

        std::shared_ptr<RaytracerState> raytracerState() const;

        virtual void update(const std::shared_ptr<StageSet>& stageSet);
        virtual void pourFramesIn(const AbstractFilm& film);

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
        virtual void hardReset();
        virtual void incorporateFilm(
                const AbstractFilm& film);

        virtual void performNonStochasticSyncronousDraf();

    private:
        static const unsigned int DEFAULT_WORKER_COUNT;

        RaytracerState::DraftParams _draftParams;
        RaytracerState::ProtectedState _protectedState;
        std::shared_ptr<RaytracerState> _raytracerState;

        bool _isUpdated;
        glm::ivec2 _viewportSize;
        std::vector<std::shared_ptr<AbstractFilm>> _films;
        std::shared_ptr<AbstractFilm> _currentFilm;

        friend class CpuRaytracerWorker;
        std::vector<std::thread> _workerThreads;
        std::vector<std::shared_ptr<CpuRaytracerWorker>> _workerObjects;
    };
}

#endif // PROPROOM3D_CPURAYTRACERENGINE_H
