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
    const double ArtDirectorServer::IMAGE_DEPTH = 400.0;
    const int ArtDirectorServer::DEFAULT_TCP_PORT = 8004;

    ArtDirectorServer::ArtDirectorServer() :
        _debugRenderer(new DebugRenderer()),
        _postProdUnit(new GlPostProdUnit()),
        _lastUpdate(TimeStamp::getCurrentTimeStamp())
    {
#ifdef NDEBUG
        _localRaytracer.reset(new CpuRaytracerEngine(8));
#else
        _localRaytracer.reset(new CpuRaytracerEngine(8));
#endif
    }

    ArtDirectorServer::~ArtDirectorServer()
    {
    }

    void ArtDirectorServer::setup(const std::shared_ptr<StageSet>& stageSet)
    {
        _stageSet = stageSet;

        RaytracerState::DraftParams draftParams;
        draftParams.sizeRatio = 4;
        draftParams.levelCount = 1;
        draftParams.frameCountPerLevel = 1;
        draftParams.fastDraftEnabled = true;

        _localRaytracer->setup(draftParams);
        _debugRenderer->setup();
        _postProdUnit->setup();

        camera()->refresh();
    }

    void ArtDirectorServer::update(double dt)
    {
        if(_stageSet->stageSetChanged(_lastUpdate))
        {
            _lastUpdate = TimeStamp::getCurrentTimeStamp();
            _postProdUnit->clearOutput();
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
        _postProdUnit->clearOutput();

        // TODO wbussiere 2015-05-01 : terminate clients
    }

    void ArtDirectorServer::resize(int width, int height)
    {
		// Image resolution is based on camera viewport size.
		// Camera viewport is independent of OpenGL viewport.
    }

    void ArtDirectorServer::notify(cellar::CameraMsg &msg)
    {
        _postProdUnit->clearOutput();
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

    std::shared_ptr<Film> ArtDirectorServer::film() const
    {
        return _localRaytracer->film();
    }

    std::string ArtDirectorServer::ipAddress() const
    {
        return "";
    }

    int ArtDirectorServer::tcpPort() const
    {
        return -1;
    }

    void ArtDirectorServer::setTcpPort(int port)
    {

    }

    bool ArtDirectorServer::isRunning() const
    {
        return false;
    }

    void ArtDirectorServer::turnOn()
    {

    }

    void ArtDirectorServer::turnOff()
    {

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
        else if(colorOuputType == RaytracerState::COLOROUTPUT_REFERENCE)
            colorOutput = Film::ColorOutput::REFERENCE;
        else if(colorOuputType == RaytracerState::COLOROUTPUT_COMPATIBILITY)
            colorOutput = Film::ColorOutput::COMPATIBILITY;

        _postProdUnit->update(*_localRaytracer->film(), colorOutput);
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
}
