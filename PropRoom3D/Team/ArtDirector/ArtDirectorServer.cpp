#include "ArtDirectorServer.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>

#include <CellarWorkbench/Camera/Camera.h>
#include <CellarWorkbench/Misc/Log.h>

#include "Film/ConvergentFilm.h"
#include "Network/ServerSocket.h"
#include "CpuRaytracerEngine.h"
#include "DebugRenderer.h"
#include "GlPostProdUnit.h"
#include "Node/StageSet.h"

using namespace cellar;


namespace prop3
{
    const double ArtDirectorServer::IMAGE_DEPTH = 400.0;
    const int ArtDirectorServer::DEFAULT_TCP_PORT = 8004;

    ArtDirectorServer::ArtDirectorServer() :
        _tcpPort(0),
        _tcpServer(nullptr),
        _film(new ConvergentFilm()),
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
        delete _tcpServer;
    }

    void ArtDirectorServer::setup(const std::shared_ptr<StageSet>& stageSet)
    {
        _stageSet = stageSet;

        RaytracerState::DraftParams draftParams;
        draftParams.sizeRatio = 4;
        draftParams.levelCount = 1;
        draftParams.frameCountPerLevel = 1;
        draftParams.fastDraftEnabled = true;

        _localRaytracer->setup(draftParams, _film);
        _debugRenderer->setup();
        _postProdUnit->setup();

        camera()->refresh();

        delete _tcpServer;
        _tcpServer = new QTcpServer();
        connect(_tcpServer, &QTcpServer::newConnection,
                this, &ArtDirectorServer::newConnection);
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

        auto it = _sockets.begin();
        while(it != _sockets.end())
        {
            if(!it->isOpen())
            {
                getLog().postMessage(new Message('W', false,
                    "ServerSocket removed", "ArtDirectorServer"));
                it = _sockets.erase(it);
            }
            else
            {
                ++it;
            }
        }
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

    void ArtDirectorServer::notify(CameraMsg &msg)
    {
        _postProdUnit->clearOutput();
        if(msg.change == CameraMsg::EChange::VIEWPORT)
        {
            const glm::ivec2& viewport = msg.camera.viewport();
            _localRaytracer->resize(viewport.x, viewport.y);
        }
        else if(msg.change == CameraMsg::EChange::PROJECTION)
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
        else if(msg.change == CameraMsg::EChange::VIEW)
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
        return _film;
    }

    std::string ArtDirectorServer::ipAddress() const
    {
        foreach(const QHostAddress &address, QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol &&
                address != QHostAddress(QHostAddress::LocalHost))
                 return address.toString().toStdString();
        }

        return "localhost";
    }

    int ArtDirectorServer::tcpPort() const
    {
        return _tcpPort;
    }

    void ArtDirectorServer::setTcpPort(int port)
    {
        if(!_tcpServer->isListening())
        {
            _tcpPort = port;
        }
        else
        {
            getLog().postMessage(new Message('W', false,
                "Cannot change TCP port when server is already listening.",
                "ArtDirectorServer"));
        }
    }

    bool ArtDirectorServer::isRunning() const
    {
        return _tcpServer->isListening();
    }

    void ArtDirectorServer::turnOn()
    {
        if(!_tcpServer->isListening())
        {
            QHostAddress address(ipAddress().c_str());
            _tcpServer->listen(address, _tcpPort);

            if(_tcpServer->isListening())
            {
                getLog().postMessage(new Message('I', false,
                    "Server up and running " +
                    toString(address.toString(), _tcpPort),
                    "ArtDirectorServer"));
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Server could not list to TCP port " +
                    toString(address.toString(), _tcpPort),
                    "ArtDirectorServer"));
            }
        }
        else
        {
            getLog().postMessage(new Message('W', false,
                "Cannot turn TCP server up, because it's already listening.",
                "ArtDirectorServer"));
        }
    }

    void ArtDirectorServer::turnOff()
    {
        if(_tcpServer->isListening())
        {
            _tcpServer->close();

            _sockets.clear();

            getLog().postMessage(new Message('W', false,
                "Server turned off.", "ArtDirectorServer"));
        }
        else
        {
            getLog().postMessage(new Message('W', false,
                "Cannot turn TCP server off, because it's not currently listening.",
                "ArtDirectorServer"));
        }
    }

    void ArtDirectorServer::newConnection()
    {
        if(_tcpServer->hasPendingConnections())
        {
            QTcpSocket* socket = _tcpServer->nextPendingConnection();
            _sockets.emplace_back(socket, _film);

            getLog().postMessage(new Message('W', false,
                "New client connected " + toString(
                    socket->peerAddress().toString(), socket->peerPort()),
                "ArtDirectorServer"));
        }
        else
        {
            getLog().postMessage(new Message('W', false,
                "New connection called, but no connection is currently pending.",
                "ArtDirectorServer"));
        }
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

        _postProdUnit->update(*_localRaytracer->currentFilm(), colorOutput);
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

        getLog().postMessage(new Message(
            'I', false, ss.str(), "ArtDirectorServer"));
    }
}
