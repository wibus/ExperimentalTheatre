#include "CpuRaytracer.h"

#include <iostream>
#include <algorithm>

#include <CellarWorkbench/Camera/Camera.h>

#include <CellarWorkbench/Misc/Log.h>

#include "../../Prop/Prop.h"
#include "../../Prop/Costume/Costume.h"
#include "../../Prop/Volume/Raycast.h"
#include "CpuRaytracerWorker.h"
#include "QGlPostProdUnit.h"


namespace prop3
{
    const unsigned int CpuRaytracer::DEFAULT_WORKER_COUNT = 4;

    CpuRaytracer::CpuRaytracer() :
        _sampleCount(0),
        _colorBufferTexId(0),
        _workerObjects(DEFAULT_WORKER_COUNT),
        _workerThreads(),
        _viewportSize(1, 1),
        _colorBuffer(3)
    {
        init();
    }

    CpuRaytracer::CpuRaytracer(unsigned int  workerCount) :
        _sampleCount(0),
        _colorBufferTexId(0),
        _workerObjects(workerCount),
        _workerThreads(),
        _viewportSize(1, 1),
        _colorBuffer(3)
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

        _postProdUnit.reset(new QGlPostProdUnit());
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

        glDeleteTextures(1, &_colorBufferTexId);
        _colorBufferTexId = 0;
    }

    void CpuRaytracer::setup()
    {
        const float black[] = {0, 0, 0};

        // Color texture
        glGenTextures(1, &_colorBufferTexId);
        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, black);
        glBindTexture(GL_TEXTURE_2D, 0);

        _postProdUnit->setColorBufferTexId(_colorBufferTexId);
        _postProdUnit->setup();
    }

    void CpuRaytracer::reset()
    {
        _props.clear();

        for(auto& w : _workerObjects)
        {
            w->setProps(_props);
        }
    }

    void CpuRaytracer::draw(double dt)
    {
        bool updated = false;


        // If there's no props, just clear OpenGL buffers
        // We are assuming that this is the first ArtDirector of the chain
        if(_props.empty())
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            return ;
        }

        // There are some props to render.

        for(std::shared_ptr<CpuRaytracerWorker>& w : _workerObjects)
        {
            while(w->completedFrameCount() != 0)
            {
                const float* pixels = w->readNextFrame();

                ++_sampleCount;
                _convergenceValue = 0.0;
                float alpha = 1.0f / _sampleCount;
                int cc = (int) _colorBuffer.size();
                for(int i=0; i < cc; ++i)
                {
                    float lastValue = _colorBuffer[i];

                    _colorBuffer[i] = glm::mix(
                        lastValue,
                        pixels[i],
                        alpha);

                    float currentValue = _colorBuffer[i];
                    float meanShift = glm::abs(currentValue - lastValue);
                    _convergenceValue += meanShift * meanShift;
                }

                w->popReadFrame();
                updated = true;
            }
        }

        if(updated)
        {
            sendBuffersToGpu();
        }

        _postProdUnit->execute();
    }

    void CpuRaytracer::notify(cellar::CameraMsg &msg)
    {
        if(msg.change == cellar::CameraMsg::EChange::VIEWPORT)
        {
            int width = msg.camera.viewport().x;
            int height = msg.camera.viewport().y;
            _colorBuffer.resize(width * height * 3);
            _viewportSize = glm::ivec2(width, height);

            for(auto& w : _workerObjects)
            {
                w->resize(width, height);
            }
        }
        else if(msg.change == cellar::CameraMsg::EChange::PROJECTION)
        {
            for(auto& w : _workerObjects)
            {
                w->updateProjection(msg.camera.projectionMatrix());
            }
        }
        else if(msg.change == cellar::CameraMsg::EChange::VIEW)
        {
            for(auto& w : _workerObjects)
            {
                w->updateView(msg.camera.viewMatrix());
            }
        }

        // Reset buffers
        std::fill(_colorBuffer.begin(), _colorBuffer.end(), 0.0f);
        _sampleCount = 0;
    }

    void CpuRaytracer::manageProp(const std::shared_ptr<Prop>& prop)
    {
        _props.push_back(prop);

        for(auto& w : _workerObjects)
        {
            w->setProps(_props);
        }

        if(_props.size() == 1)
        {
            _postProdUnit->show();
        }
    }

    void CpuRaytracer::unmanageProp(const std::shared_ptr<Prop>& prop)
    {
        std::remove_if(_props.begin(), _props.end(),
            [&prop](const std::shared_ptr<Prop>& p) {
                return p == prop;
        });

        for(auto& w : _workerObjects)
        {
            w->setProps(_props);
        }

        if(_props.size() == 0)
        {
            _postProdUnit->hide();
        }
    }

    void CpuRaytracer::sendBuffersToGpu()
    {
        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);
        glTexImage2D(GL_TEXTURE_2D,     0,  GL_RGB,
                     _viewportSize.x,       _viewportSize.y,
                     0, GL_RGB, GL_FLOAT,   _colorBuffer.data());

        float sum = 0.0f;
        for(float c : _colorBuffer)
            sum += c;
        std::cout << "Sample count = " << _sampleCount <<
                     " (convergence = " << _convergenceValue << ")" << std::endl;
    }
}
