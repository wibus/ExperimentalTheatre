#include "RaytracerState.h"

#include <numeric>


namespace prop3
{
    const std::string RaytracerState::COLOROUTPUT_ALBEDO = "Albedo";
    const std::string RaytracerState::COLOROUTPUT_WEIGHT = "Weight";
    const std::string RaytracerState::COLOROUTPUT_DIVERGENCE = "Divergence";
    const std::string RaytracerState::COLOROUTPUT_VARIANCE = "Variance";
    const std::string RaytracerState::COLOROUTPUT_PRIORITY = "Priority";
    const std::string RaytracerState::COLOROUTPUT_REFERENCE = "Reference";
    const std::string RaytracerState::COLOROUTPUT_COMPATIBILITY = "Compatiblity";

    const std::string RaytracerState::UNSPECIFIED_RAW_FILE = "";


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
        _hiddenSurfacesRemoved(false),
        _startTime(std::chrono::steady_clock::now()),
        _sampleCount(0),
        _divergence(1.0),
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

    void RaytracerState::ProtectedState::setHiddenSurfaceRemoved(bool removed)
    {
        _hiddenSurfacesRemoved = removed;
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
        _protectedState(state),
        _isUpdateEachTileEnabled(true),
        _colorOutputType(COLOROUTPUT_ALBEDO),
        _sampleCountThreshold(std::numeric_limits<unsigned int>::max()),
        _renderTimeThreshold(std::numeric_limits<double>::infinity()),
        _divergenceThreshold(-1.0),
        _surfaceVisibilityThreshold(0.02)
    {

    }

    void RaytracerState::setDivergenceThreshold(double divergenceThreshold)
    {
        _divergenceThreshold = divergenceThreshold;
    }

    void RaytracerState::setSampleCountThreshold(unsigned int sampleCountThreshold)
    {
        _sampleCountThreshold = sampleCountThreshold;
    }

    void RaytracerState::setRenderTimeThreshold(double renderTimeThreshold)
    {
        _renderTimeThreshold = renderTimeThreshold;
    }

    void RaytracerState::setSurfaceVisibilityThreshold(double threshold)
    {
        _surfaceVisibilityThreshold = threshold;
    }

    void RaytracerState::setUpdateEachTile(bool enabled)
    {
        _isUpdateEachTileEnabled = enabled;
    }

    void RaytracerState::setColorOutputType(const std::string& colorOutput)
    {
        _colorOutputType = colorOutput;
    }

    void RaytracerState::setFilmRawFilePath(const std::string& filePath)
    {
        _filmRawFilePath = filePath;
    }
}
