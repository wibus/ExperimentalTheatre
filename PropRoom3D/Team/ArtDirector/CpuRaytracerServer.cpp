#include "CpuRaytracerServer.h"

#include <iostream>
#include <sstream>

#include <CellarWorkbench/Camera/Camera.h>
#include <CellarWorkbench/Misc/Log.h>

#include "CpuRaytracer.h"
#include "QGlPostProdUnit.h"


namespace prop3
{
    CpuRaytracerServer::CpuRaytracerServer() :
        _colorBufferTexId(0),
        _localRaytracer(new CpuRaytracer(4)),
        _postProdUnit(new QGlPostProdUnit())
    {
    }

    CpuRaytracerServer::~CpuRaytracerServer()
    {
        glDeleteTextures(1, &_colorBufferTexId);
        _colorBufferTexId = 0;
    }

    void CpuRaytracerServer::setup()
    {
        // Color texture
        glGenTextures(1, &_colorBufferTexId);
        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        clearColorTexture();

        _localRaytracer->enableFastDraft(true);
        _localRaytracer->setDraftParams(2, 4, 1);
        _postProdUnit->setColorBufferTexId(_colorBufferTexId);
        _postProdUnit->setup();
    }

    void CpuRaytracerServer::reset()
    {
        _localRaytracer->reset();

        // TODO wbussiere 2015-05-01 : reset clients
    }

    void CpuRaytracerServer::draw(double dt)
    {
        _localRaytracer->gatherWorkerFrames();

        // TODO wbussiere 2015-05-01 : retreive client frames


        if(_localRaytracer->isUpdated())
        {
            sendBuffersToGpu();
            // Let raytracer manage its drafts
            _localRaytracer->onUpdateConsumed();
        }

        _postProdUnit->execute();
    }

    void CpuRaytracerServer::notify(cellar::CameraMsg &msg)
    {
        clearColorTexture();
        if(msg.change == cellar::CameraMsg::EChange::VIEWPORT)
        {
            const glm::ivec2& viewport = msg.camera.viewport();
            _localRaytracer->resize(viewport.x, viewport.y);
        }
        else if(msg.change == cellar::CameraMsg::EChange::PROJECTION)
        {
            _localRaytracer->updateProjection(msg.camera.projectionMatrix());
        }
        else if(msg.change == cellar::CameraMsg::EChange::VIEW)
        {
            _localRaytracer->updateView(msg.camera.viewMatrix());
        }


        // TODO wbussiere 2015-05-01 : notify clients
    }

    void CpuRaytracerServer::manageProp(const std::shared_ptr<Prop>& prop)
    {
        clearColorTexture();
        _localRaytracer->manageProp(prop);

        // TODO wbussiere 2015-05-01 : notify clients


        if(_localRaytracer->propCount() > 0)
        {
            _postProdUnit->show();
        }
    }

    void CpuRaytracerServer::unmanageProp(const std::shared_ptr<Prop>& prop)
    {
        clearColorTexture();
        _localRaytracer->unmanageProp(prop);

        // TODO wbussiere 2015-05-01 : notify clients


        if(_localRaytracer->propCount() == 0)
        {
            _postProdUnit->hide();
        }
    }

    void CpuRaytracerServer::sendBuffersToGpu()
    {
        const glm::ivec2& viewportSize = _localRaytracer->viewportSize();
        const std::vector<float>& colorBuffer = _localRaytracer->colorBuffer();


        // Send image to GPU
        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);
        glTexImage2D(GL_TEXTURE_2D,         0,  GL_RGB,
                     viewportSize.x,        viewportSize.y,
                     0, GL_RGB, GL_FLOAT,   colorBuffer.data());


        // Output image stats
        std::stringstream ss;
        bool isDraft = _localRaytracer->isDrafting();
        float convergence = _localRaytracer->convergenceValue();

        ss << "Sample count = " << _localRaytracer->sampleCount() <<
              " -> convergence = " << std::scientific << convergence <<
              (isDraft ? "[Draft]" : "");

        cellar::getLog().postMessage(new cellar::Message(
            'I', false, ss.str(), "CpuRaytracerServer"));
    }

    void CpuRaytracerServer::clearColorTexture()
    {
        const float black[] = {0, 0, 0};

        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, black);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
