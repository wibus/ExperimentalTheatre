#include "RaytracerState.h"


namespace prop3
{
    RaytracerState::ProtectedState::ProtectedState() :
        _workerCount(0),
        _interrupted(true),
        _startTime(std::chrono::steady_clock::now()),
        _sampleCount(0),
        _divergence(1.0),
        _draftLevel(0)
    {

    }

    double RaytracerState::ProtectedState::renderTime() const
    {
        std::chrono::duration<double> dt =
            std::chrono::steady_clock::now() - _startTime;

        return dt.count();
    }

    void RaytracerState::ProtectedState::startTimeChrono()
    {
        _startTime = std::chrono::steady_clock::now();
    }

    void RaytracerState::ProtectedState::setWorkerCount(int workerCount)
    {
        _workerCount = workerCount;
    }

    void RaytracerState::ProtectedState::setInterrupted(bool interrupted)
    {
        _interrupted = interrupted;
    }

    void RaytracerState::ProtectedState::setSampleCount(unsigned int sampleCount)
    {
        _sampleCount = sampleCount;
    }

    void RaytracerState::ProtectedState::setDivergence(double divergence)
    {
        _divergence = divergence;
    }

    void RaytracerState::ProtectedState::setDraftLevel(int draftLevel)
    {
        _draftLevel = draftLevel;
    }


    RaytracerState::RaytracerState(ProtectedState& state) :
        _protectedState(state),
        _divergenceThreshold(-1.0),
        _draftLevelCount(0),
        _draftLevelSizeRatio(1),
        _draftFrameCountPerLevel(1),
        _fastDraftEnabled(false)
    {

    }

    void RaytracerState::setDivergenceThreshold(double divergenceThreshold)
    {
        _divergenceThreshold = divergenceThreshold;
    }

    void RaytracerState::setDraftParams(
                    int levelCount,
                    int levelSizeRatio,
                    int frameCountPerLevel,
                    bool enableFastDraft)
    {
        _draftLevelCount = levelCount;
        _draftLevelSizeRatio = levelSizeRatio;
        _draftFrameCountPerLevel = frameCountPerLevel;
        _fastDraftEnabled = enableFastDraft;
    }
}
