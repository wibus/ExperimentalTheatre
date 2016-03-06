#include "ArtDirectorServer.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include <CellarWorkbench/Camera/Camera.h>
#include <CellarWorkbench/Misc/Log.h>

#include "CpuRaytracerEngine.h"
#include "DebugRenderer.h"
#include "Film/Film.h"
#include "GlPostProdUnit.h"
#include "Node/StageSet.h"


namespace prop3
{
    const double ArtDirectorServer::FORCE_REFRESH_DT = 0.0;
    const double ArtDirectorServer::IMAGE_DEPTH = 400.0;

    ArtDirectorServer::ArtDirectorServer() :
        _colorBufferTexId(0),
        _debugRenderer(new DebugRenderer()),
        _postProdUnit(new GlPostProdUnit())
    {
#ifdef NDEBUG
        _localRaytracer.reset(new CpuRaytracerEngine(8));
#else
        _localRaytracer.reset(new CpuRaytracerEngine(8));
#endif
    }

    ArtDirectorServer::~ArtDirectorServer()
    {
        glDeleteTextures(1, &_colorBufferTexId);
        _colorBufferTexId = 0;
    }

    void ArtDirectorServer::setup(const std::shared_ptr<StageSet>& stageSet)
    {
        // Color texture
        glGenTextures(1, &_colorBufferTexId);
        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Depth texture
        glGenTextures(1, &_depthBufferTexId);
        glBindTexture(GL_TEXTURE_2D, _depthBufferTexId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        clearTextures();


        _stageSet = stageSet;

        RaytracerState::DraftParams draftParams;
        draftParams.sizeRatio = 4;
        draftParams.levelCount = 1;
        draftParams.frameCountPerLevel = 1;
        draftParams.fastDraftEnabled = true;

        _localRaytracer->setup(draftParams);

        if(_postProdUnit)
        {
            _postProdUnit->setColorBufferTexId(_colorBufferTexId);
            _postProdUnit->setDepthBufferTexId(_depthBufferTexId);
            _postProdUnit->setup();
        }

        _debugRenderer->setup();

        camera()->refresh();
    }

    void ArtDirectorServer::update(double dt)
    {
        if(_stageSet->stageSetChanged())
        {
            clearTextures();
        }

        _localRaytracer->update(_stageSet);

        // TODO wbussiere 2015-05-01 : retreive client frames
        //_localRaytracer->pourFramesIn();
    }

    void ArtDirectorServer::draw(double dt)
    {
        if(dt == FORCE_REFRESH_DT ||
           _localRaytracer->newTileCompleted())
        {
            if(dt == FORCE_REFRESH_DT ||
               raytracerState()->isUpdateEachTileEnabled())
                sendBuffersToGpu();

            if(_localRaytracer->newFrameCompleted())
            {
                if(!raytracerState()->isUpdateEachTileEnabled())
                    sendBuffersToGpu();

                // Let raytracer manage its drafts
                _localRaytracer->manageNextFrame();
                printConvergence();
            }
        }

        _postProdUnit->execute();

        _debugRenderer->draw(_stageSet);
    }

    void ArtDirectorServer::terminate()
    {
        _localRaytracer->terminate();
        clearTextures();

        // TODO wbussiere 2015-05-01 : terminate clients
    }

    void ArtDirectorServer::resize(int width, int height)
    {
		// Image resolution is based on camera viewport size.
		// Camera viewport is independent of OpenGL viewport.
    }

    void ArtDirectorServer::notify(cellar::CameraMsg &msg)
    {
        clearTextures();
        if(msg.change == cellar::CameraMsg::EChange::VIEWPORT)
        {
            const glm::ivec2& viewport = msg.camera.viewport();
            _localRaytracer->resize(viewport.x, viewport.y);
        }
        else if(msg.change == cellar::CameraMsg::EChange::PROJECTION)
        {
            const glm::mat4& proj = msg.camera.projectionMatrix();
            _localRaytracer->updateProjection(proj);
            _debugRenderer->updateProjection(proj);

            glm::mat4 invProj = glm::inverse(proj);
            glm::vec4 nearVec = invProj * glm::vec4(0, 0, -1, 1);
            glm::vec4 farVec = invProj * glm::vec4(0, 0, 1, 1);
            glm::vec2 depthRange(
                -nearVec.z / nearVec.w,
                -farVec.z / farVec.w);

            _postProdUnit->updateDepthRange(depthRange);

        }
        else if(msg.change == cellar::CameraMsg::EChange::VIEW)
        {
            _localRaytracer->updateView(msg.camera.viewMatrix());
            _debugRenderer->updateView(msg.camera.viewMatrix());
        }


        // TODO wbussiere 2015-05-01 : notify clients
    }

    std::shared_ptr<GlPostProdUnit> ArtDirectorServer::postProdUnit() const
    {
        return _postProdUnit;
    }

    std::shared_ptr<RaytracerState> ArtDirectorServer::raytracerState() const
    {
        return _localRaytracer->raytracerState();
    }

    void ArtDirectorServer::sendBuffersToGpu()
    {
        std::string colorOuputType = raytracerState()->colorOutputType();
        Film::ColorOutput colorOutput = Film::ColorOutput::ALBEDO;
        if(colorOuputType == RaytracerState::COLOROUTPUT_WEIGHT)
            colorOutput = Film::ColorOutput::WEIGHT;
        if(colorOuputType == RaytracerState::COLOROUTPUT_DIVERGENCE)
            colorOutput = Film::ColorOutput::DIVERGENCE;
        else if(colorOuputType == RaytracerState::COLOROUTPUT_VARIANCE)
            colorOutput = Film::ColorOutput::VARIANCE;
        else if(colorOuputType == RaytracerState::COLOROUTPUT_PRIORITY)
            colorOutput = Film::ColorOutput::PRIORITY;

        auto film = _localRaytracer->film();
        glm::ivec2 viewportSize = film->frameResolution();
        const std::vector<float>& depthBuffer = film->depthBuffer();
        const std::vector<glm::vec3>& colorBuffer = film->colorBuffer(colorOutput);

        // Send image to GPU
        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);
        glTexImage2D(GL_TEXTURE_2D,         0,  GL_RGB32F,
                     viewportSize.x,        viewportSize.y,
                     0, GL_RGB, GL_FLOAT,   colorBuffer.data());

        if(!depthBuffer.empty())
        {
            glBindTexture(GL_TEXTURE_2D, _depthBufferTexId);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F,
                         viewportSize.x,        viewportSize.y,
                         0, GL_RED, GL_FLOAT, depthBuffer.data());
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void ArtDirectorServer::printConvergence()
    {
        // Output image stats
        unsigned int sampleCount = raytracerState()->sampleCount();

        if(sampleCount <= 0)
            return;

        std::stringstream ss;
        ss << /*"Frame " <<*/ sampleCount;

        if(!raytracerState()->isDrafting())
        {
            //* Human readable
            float renderTime = raytracerState()->renderTime();
            float secPerFrame = renderTime / sampleCount;
            float divergence = raytracerState()->divergence();

            ss << "\t(";
            ss.precision(3);
            ss << std::fixed << std::setw(10) << renderTime << " sec";
            ss << ", ";
            ss.precision(3);
            ss << std::fixed << std::setw(10)  << secPerFrame << " sec/frame";
            ss << ", ";
            ss.precision(3);
            ss << std::scientific << std::setw(10) << divergence << " div";
            ss << ")";
            //*/

            /* CVS File
            float renderTime = _localRaytracer->renderTime();
            float secPerFrame = renderTime / sampleCount;
            float divergence = _localRaytracer->divergenceValue();
            float imgVariance = _localRaytracer->imageVariance();

            ss << ", ";
            ss.precision(3);
            ss << std::fixed << std::setw(10) << renderTime << ", ";
            ss.precision(3);
            ss << std::fixed << std::setw(10)  << secPerFrame << ", ";
            ss.precision(3);
            ss << std::scientific << std::setw(10) << divergence << ", ";
            ss.precision(3);
            ss << std::scientific << std::setw(10) << imgVariance << ";";
            //*/
        }
        else
        {
            ss << " [Draft]";
        }

        cellar::getLog().postMessage(new cellar::Message(
            'I', false, ss.str(), "CpuRaytracerServer"));
    }

    void ArtDirectorServer::clearTextures()
    {
        const float maxDepth = INFINITY;
        const float black[] = {0, 0, 0};

        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, black);

        glBindTexture(GL_TEXTURE_2D, _depthBufferTexId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 1, 1, 0,
                     GL_RED, GL_FLOAT, &maxDepth);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
