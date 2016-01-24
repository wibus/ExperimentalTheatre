#include "RaytracerState.h"

#include <numeric>


namespace prop3
{
    RaytracerState::DraftParams::DraftParams() :
        levelCount(0),
        sizeRatio(1),
        frameCountPerLevel(1),
        fastDraftEnabled(false)
    {

    }

    RaytracerState::DraftParams::DraftParams(
            int levelCount,
            int levelSizeRatio,
            int frameCountPerLevel,
            bool fastDraftEnabled) :
        levelCount(levelCount),
        sizeRatio(levelSizeRatio),
        frameCountPerLevel(frameCountPerLevel),
        fastDraftEnabled(fastDraftEnabled)
    {

    }

    RaytracerState::ProtectedState::ProtectedState() :
        _workerCount(0),
        _interrupted(true),
        _startTime(std::chrono::steady_clock::now()),
        _renderTimeThreshold(std::numeric_limits<double>::infinity()),
        _sampleCount(0),
        _sampleCountThreshold(std::numeric_limits<unsigned int>::max()),
        _divergence(1.0),
        _divergenceThreshold(-1.0),
        _draftLevel(0),
        _draftParams()
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

    void RaytracerState::ProtectedState::incSampleCount()
    {
        ++_sampleCount;
    }

    void RaytracerState::ProtectedState::addSampleCount(unsigned int count)
    {
        _sampleCount += count;
    }

    void RaytracerState::ProtectedState::resetSampleCount()
    {
        _sampleCount = 0;
    }

    void RaytracerState::ProtectedState::setDivergenceThreshold(double divergenceThreshold)
    {
        _divergenceThreshold = divergenceThreshold;
    }

    void RaytracerState::ProtectedState::setSampleCountThreshold(unsigned int sampleCountThreshold)
    {
        _sampleCountThreshold = sampleCountThreshold;
    }

    void RaytracerState::ProtectedState::setRenderTimeThreshold(double renderTimeThreshold)
    {
        _renderTimeThreshold = renderTimeThreshold;
    }

    void RaytracerState::ProtectedState::setDivergence(double divergence)
    {
        _divergence = divergence;
    }

    void RaytracerState::ProtectedState::setDraftParams(const DraftParams& draftParams)
    {
        _draftParams = draftParams;
    }

    void RaytracerState::ProtectedState::setDraftLevel(int draftLevel)
    {
        _draftLevel = draftLevel;
    }


    RaytracerState::RaytracerState(ProtectedState& state) :
        _protectedState(state)
    {

    }
}
