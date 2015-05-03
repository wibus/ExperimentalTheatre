#include "CpuRaytracer.h"

#include <iostream>
#include <algorithm>

#include "../../Prop/Prop.h"
#include "../../Prop/Costume/Costume.h"
#include "../../Prop/Volume/Raycast.h"
#include "CpuRaytracerWorker.h"


namespace prop3
{
    const unsigned int CpuRaytracer::DEFAULT_WORKER_COUNT = 4;

    CpuRaytracer::CpuRaytracer() :
        _sampleCount(0),
        _draftLevel(0),
        _draftLevelCount(0),
        _draftThreadBatchPerLevel(0),
        _draftViewport(1, 1),
        _isUpdated(false),
        _viewportSize(1, 1),
        _workerThreads(),
        _workerObjects(DEFAULT_WORKER_COUNT)
    {
        init();
    }

    CpuRaytracer::CpuRaytracer(unsigned int  workerCount) :
        _sampleCount(0),
        _draftLevel(0),
        _draftLevelCount(0),
        _draftThreadBatchPerLevel(0),
        _draftViewport(1, 1),
        _isUpdated(false),
        _viewportSize(1, 1),
        _workerThreads(),
        _workerObjects(workerCount)
    {
        init();
    }


    void CpuRaytracer::init()
    {
        size_t workerCount = _workerObjects.size();
        for(size_t i=0; i < workerCount; ++i)
        {
            _workerObjects[i].reset(new CpuRaytracerWorker());
            _workerThreads.push_back(std::thread(
                CpuRaytracerWorker::launchWorker,
                _workerObjects[i]));
        }

        bufferHardReset();
    }

    CpuRaytracer::~CpuRaytracer()
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

    void CpuRaytracer::reset()
    {
        _props.clear();

        for(auto& w : _workerObjects)
        {
            w->setProps(_props);
        }

        bufferSoftReset();
    }

    bool CpuRaytracer::isDrafter() const
    {
        return _draftLevelCount != 0 && _draftThreadBatchPerLevel != 0;
    }

    bool CpuRaytracer::isDrafting() const
    {
        return isDrafter() && _draftLevel < _draftLevelCount;
    }

    void CpuRaytracer::setDraftParams(
            int levelCount,
            int levelSizeRatio,
            int threadBatchPerLevel)
    {
        if(isDrafting())
            abortDrafting();

        _draftLevelCount = levelCount;
        _draftLevelSizeRatio = levelSizeRatio;
        _draftThreadBatchPerLevel = threadBatchPerLevel;

        if(isDrafter())
            restartDrafting();
    }

    void CpuRaytracer::gatherWorkerFrames()
    {
        if(_props.empty())
            return;

        for(std::shared_ptr<CpuRaytracerWorker>& w : _workerObjects)
        {
            while(w->completedFrameCount() != 0)
            {
                const float* pixels = w->readNextFrame();
                incorporateFrames(pixels, 1);
                w->popReadFrame();
            }
        }
    }

    void CpuRaytracer::pourFramesIn(
            const std::vector<float>& colorBuffer,
            unsigned int sampleCount)
    {
        abortDrafting();
        incorporateFrames(colorBuffer.data(), sampleCount);
    }

    void CpuRaytracer::pourFramesOut(
            std::vector<float>& colorBuffer,
            unsigned int& sampleCount)
    {
        sampleCount = _sampleCount;
        colorBuffer.resize(_colorBuffer.size());
        std::copy(_colorBuffer.begin(), _colorBuffer.end(), colorBuffer.begin());

        _isUpdated = false;
        bufferSoftReset();
    }

    bool CpuRaytracer::isUpdated()
    {
        return _isUpdated;
    }

    bool CpuRaytracer::onUpdateConsumed()
    {
        _isUpdated = false;

        if(isDrafting())
        {
            if(_sampleCount >= _draftThreadBatchPerLevel * _workerThreads.size())
            {
                nextDraftSize();
            }
        }
    }

    float CpuRaytracer::convergenceValue() const
    {
        return _convergenceValue;
    }

    unsigned int CpuRaytracer::sampleCount() const
    {
        return _sampleCount;
    }

    const glm::ivec2& CpuRaytracer::viewportSize() const
    {
        if(isDrafting())
            return _draftViewport;
        return _viewportSize;
    }

    const std::vector<float>& CpuRaytracer::colorBuffer() const
    {
        return _colorBuffer;
    }

    void CpuRaytracer::resize(int width, int height)
    {
        _viewportSize = glm::ivec2(width, height);
        _colorBuffer.resize(width * height * 3);

        bufferSoftReset();

        if(!isDrafting())
        {
            for(auto& w : _workerObjects)
            {
                w->resize(width, height);
            }
        }
    }

    void CpuRaytracer::updateView(const glm::dmat4& view)
    {
        bufferSoftReset();

        for(auto& w : _workerObjects)
        {
            w->updateView(view);
        }
    }

    void CpuRaytracer::updateProjection(const glm::dmat4& proj)
    {
        bufferSoftReset();

        for(auto& w : _workerObjects)
        {
            w->updateProjection(proj);
        }
    }

    unsigned int CpuRaytracer::propCount() const
    {
        return _props.size();
    }

    void CpuRaytracer::manageProp(const std::shared_ptr<Prop>& prop)
    {
        _props.push_back(prop);

        bufferSoftReset();

        for(auto& w : _workerObjects)
        {
            w->setProps(_props);
        }
    }

    void CpuRaytracer::unmanageProp(const std::shared_ptr<Prop>& prop)
    {
        std::remove_if(_props.begin(), _props.end(),
            [&prop](const std::shared_ptr<Prop>& p) {
                return p == prop;
        });

        bufferSoftReset();

        for(auto& w : _workerObjects)
        {
            w->setProps(_props);
        }
    }

    void CpuRaytracer::nextDraftSize()
    {
        if(!isDrafting())
            return;

        ++_draftLevel;
        _sampleCount = 0;
        _convergenceValue = 0;

        if(_draftLevel < _draftLevelCount)
        {
            int ratioPower = (_draftLevelCount - (_draftLevel+1));
            int ratio = glm::pow(2, ratioPower) * _draftLevelSizeRatio;

            _draftViewport = _viewportSize / glm::ivec2(ratio);
            _draftViewport = glm::max(_draftViewport, glm::ivec2(1));
        }
        else
        {
            _draftViewport = _viewportSize;
        }

        // Update worker buffers' size
        for(auto& w : _workerObjects)
        {
            w->resize(_draftViewport.x, _draftViewport.y);
        }
    }

    void CpuRaytracer::abortDrafting()
    {
        if(!isDrafting())
            return;

        _draftLevel = _draftLevelCount-1;
        nextDraftSize();
    }

    void CpuRaytracer::restartDrafting()
    {
        if(!isDrafter())
            return;

        _draftLevel = -1;
        nextDraftSize();
    }

    void CpuRaytracer::bufferSoftReset()
    {
        _sampleCount = 0;
        restartDrafting();
    }

    void CpuRaytracer::bufferHardReset()
    {
        _sampleCount = 0;
        restartDrafting();

        _colorBuffer.resize(_viewportSize.x * _viewportSize.y * 3);
        std::fill(_colorBuffer.begin(), _colorBuffer.end(), 0.0f);
    }

    void CpuRaytracer::incorporateFrames(
        const float* colorBuffer,
        unsigned int sampleCount)
    {
        _convergenceValue = 0.0;
        _sampleCount += sampleCount;
        float alpha = sampleCount / (float) _sampleCount;

        const glm::ivec2& viewport = viewportSize();
        int cc = viewport.x * viewport.y * 3;
        for(int i=0; i < cc; ++i)
        {
            float lastValue = _colorBuffer[i];
            float newValue = _colorBuffer[i] =
                glm::mix(
                    _colorBuffer[i],
                    colorBuffer[i],
                    alpha);

            float meanShift = glm::abs(newValue - lastValue);
            _convergenceValue += (meanShift * meanShift);
        }

        _convergenceValue = glm::sqrt(_convergenceValue) * sampleCount;
        _convergenceValue = 1.0f / (1.0f + _convergenceValue);
        _isUpdated = true;
    }
}
