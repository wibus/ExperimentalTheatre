#include "CpuRaytracer.h"

#include <algorithm>

#include <CellarWorkbench/Misc/CellarUtils.h>
#include <CellarWorkbench/Misc/Log.h>

#include <MediaWorkbench/Camera/Camera.h>

#include "../../Prop/Prop.h"
#include "../../Prop/Costume/Costume.h"
#include "../../Prop/Volume/Raycast.h"
#include "CpuRaytracerWorker.h"
#include "QGlPostProdUnit.h"


namespace prop3
{
    const int CpuRaytracer::WORKER_COUNT = 4;

    CpuRaytracer::CpuRaytracer() :
        _sampleCount(0),
        _colorBufferTexId(0),
        _workerObjects(WORKER_COUNT),
        _workerThreads(),
        _viewportSize(1, 1),
        _colorBuffer(3)
    {
        for(int i=0; i < WORKER_COUNT; ++i)
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
            w->skip();
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
        for(auto& w : _workerObjects)
        {
            w->skip();
        }

        _props.clear();

        for(auto& w : _workerObjects)
        {
            w->setProps(_props);
        }
    }

    void CpuRaytracer::draw(double dt)
    {
        bool updated = false;

        for(std::shared_ptr<CpuRaytracerWorker>& w : _workerObjects)
        {
            if(w->tryLockPixels())
            {
                if(w->frameIsComplete())
                {
                    const std::vector<float> pixels = w->pixels();

                    ++_sampleCount;
                    float alpha = 1.0f / _sampleCount;
                    int cc = (int) _colorBuffer.size();
                    for(int i=0; i < cc; ++i)
                    {
                        _colorBuffer[i] = glm::mix(_colorBuffer[i], pixels[i], alpha);
                    }

                    updated = true;
                }

                w->unlockPixels();
                w->nextFrame();
            }
        }

        if(updated)
        {
            sendBuffersToGpu();
        }

        _postProdUnit->execute();
    }

    void CpuRaytracer::notify(media::CameraMsg &msg)
    {
        if(msg.change == media::CameraMsg::EChange::PROJECTION)
        {
            for(auto& w : _workerObjects)
            {
                w->skip();
            }

            int width = msg.camera.viewport().x();
            int height = msg.camera.viewport().y();
            _viewportSize = glm::ivec2(width, height);
            _colorBuffer.resize(width * height * 3);
            std::fill(_colorBuffer.begin(), _colorBuffer.end(), 0.0f);
            _sampleCount = 0;

            for(auto& w : _workerObjects)
            {
                w->resize(width, height);
            }
        }
    }

    void CpuRaytracer::manageProp(const std::shared_ptr<Prop>& prop)
    {
        for(auto& w : _workerObjects)
        {
            w->skip();
        }

        _props.push_back(prop);

        for(auto& w : _workerObjects)
        {
            w->setProps(_props);
        }
    }

    void CpuRaytracer::unmanageProp(const std::shared_ptr<Prop>& prop)
    {
        for(auto& w : _workerObjects)
        {
            w->skip();
        }

        std::remove_if(_props.begin(), _props.end(),
            [&prop](const std::shared_ptr<Prop>& p) {
                return p == prop;
        });

        for(auto& w : _workerObjects)
        {
            w->setProps(_props);
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
                     " (sum = " << sum << ")" << std::endl;
    }
}
