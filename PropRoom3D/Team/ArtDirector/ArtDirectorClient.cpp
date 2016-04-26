#include "ArtDirectorClient.h"

#include <QTcpSocket>
#include <QNetworkInterface>

#include <CellarWorkbench/Misc/Log.h>

#include "Film/NetworkFilm.h"
#include "Network/TileMessage.h"
#include "Network/UpdateMessage.h"
#include "CpuRaytracerEngine.h"
#include "GlPostProdUnit.h"

using namespace cellar;


namespace prop3
{
    ArtDirectorClient::ArtDirectorClient() :
        _socket(new QTcpSocket(this)),
        _film(new NetworkFilm()),
        _postProdUnit(new GlPostProdUnit()),
        _isConnected(false)
    {
#ifdef NDEBUG
        _localRaytracer.reset(new CpuRaytracerEngine(8));
#else
        _localRaytracer.reset(new CpuRaytracerEngine(8));
#endif

        _socket->setReadBufferSize(0);

        connect(_socket, &QTcpSocket::connected,
                this, &ArtDirectorClient::connected);

        connect(_socket, &QTcpSocket::disconnected,
                this, &ArtDirectorClient::disconected);

        connect(_socket, &QTcpSocket::readyRead,
                this, &ArtDirectorClient::readMessage);
    }

    ArtDirectorClient::~ArtDirectorClient()
    {

    }

    void ArtDirectorClient::setup(const std::shared_ptr<StageSet>& stageSet)
    {
        _stageSet = stageSet;

        RaytracerState::DraftParams draftParams;
        draftParams.sizeRatio = 0;
        draftParams.levelCount = 0;
        draftParams.frameCountPerLevel = 0;
        draftParams.fastDraftEnabled = false;

        _localRaytracer->setup(draftParams, _film);
        _postProdUnit->setup();
    }

    void ArtDirectorClient::update(double dt)
    {
        if(_updateMessage.get() != nullptr)
        {
            getLog().postMessage(new Message('I', false,
                "Consuming Update message",
                "ArtDirectorClient"));

            _localRaytracer->interrupt();

            _film->setStateUid(_updateMessage->uid);

            _localRaytracer->updateStageSet(
                _updateMessage->stageSetStream);

            if(_updateMessage->proj != camera()->projectionMatrix())
            {
                camera()->updateProjection(_updateMessage->proj);

                _localRaytracer->updateProjection(
                    _updateMessage->proj);
            }

            if(_updateMessage->view != camera()->viewMatrix())
            {
                camera()->updateView(_updateMessage->view);

                _localRaytracer->updateView(
                    _updateMessage->view);
            }

            if(_updateMessage->viewport != camera()->viewport())
            {
                camera()->updateViewport(_updateMessage->viewport);

                _localRaytracer->resize(
                    _updateMessage->viewport.x,
                    _updateMessage->viewport.y);
            }

            _updateMessage.reset();
        }

        _localRaytracer->update();

        sendTilesToServer();
    }

    void ArtDirectorClient::draw(double dt)
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
            }
        }

        _postProdUnit->execute();
    }

    void ArtDirectorClient::terminate()
    {

    }

    void ArtDirectorClient::resize(int width, int height)
    {
    }

    void ArtDirectorClient::notify(cellar::CameraMsg &msg)
    {

    }

    std::shared_ptr<RaytracerState> ArtDirectorClient::raytracerState() const
    {
        return _localRaytracer->raytracerState();
    }

    bool ArtDirectorClient::isConnected() const
    {
        return _isConnected;
    }

    void ArtDirectorClient::connectToServer()
    {
        if(!_socket->isOpen())
        {
            QHostAddress address(_serverIpAddress.c_str());
            _socket->connectToHost(address, _serverTcpPort);

            if(!_socket->waitForConnected())
            {
                getLog().postMessage(new Message('E', false,
                    "Could not reach the server " +
                    toString(address.toString(), _serverTcpPort),
                    "ArtDirectorClient"));

                getLog().postMessage(new Message('E', false,
                    _socket->errorString().toStdString(),
                    "ArtDirectorClient"));

                _socket->close();
            }
        }
        else
        {
            getLog().postMessage(new Message('W', false,
                "A connection is already established with the server",
                "ArtDirectorClient"));
        }
    }

    void ArtDirectorClient::disconnectFromServer()
    {
        if(_socket->isOpen())
        {
            _socket->close();
            _isConnected = false;
            _film->setStateUid(-1);

            _localRaytracer->updateStageSet("");
            _film->clear(glm::dvec3(0.0), true);

            getLog().postMessage(new Message('I', false,
                "Server connection closed",
                "ArtDirectorClient"));
        }
        else
        {
            getLog().postMessage(new Message('W', false,
                "No connection is currently established with the server",
                "ArtDirectorClient"));
        }
    }

    void ArtDirectorClient::setServerTcpPort(int port)
    {
        _serverTcpPort = port;
    }

    void ArtDirectorClient::setServerIpAddress(const std::string& ip)
    {
        _serverIpAddress = ip;
    }

    void ArtDirectorClient::connected()
    {
        _isConnected = true;

        getLog().postMessage(new Message('I', false,
            "New connection established with server",
            "ArtDirectorClient"));
    }

    void ArtDirectorClient::disconected()
    {
        _socket->close();
        _isConnected = false;
        _film->setStateUid(-1);

        _localRaytracer->updateStageSet("");
        _film->clear(glm::dvec3(0.0), true);

        getLog().postMessage(new Message('I', false,
            "Server disconnected from client",
            "ArtDirectorClient"));
    }

    void ArtDirectorClient::readMessage()
    {
        while(_socket->bytesAvailable() > 0)
        {
            _updateMessage.reset(new UpdateMessage(*_socket));

            if(!_updateMessage->isComplete())
            {
                getLog().postMessage(new Message('E', false,
                    "Incomplete message received",
                    "ArtDirectorClient"));
            }
        }
    }

    void ArtDirectorClient::sendBuffersToGpu()
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

    void ArtDirectorClient::sendTilesToServer()
    {
        size_t processedTileCount = 0;
        size_t filmTileCount = _film->tileCount();

        std::shared_ptr<TileMessage> msg;
        while((msg = _film->nextOutgoingTile()).get() != nullptr &&
              ++processedTileCount <= filmTileCount)
        {
            msg->write(*_socket);
        }

        if(processedTileCount > filmTileCount)
        {
            getLog().postMessage(new Message('I', false,
                "Network is too slow compared to tile completion",
                "ArtDirectorClient"));
        }
    }
}
