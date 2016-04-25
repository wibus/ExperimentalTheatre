#ifndef PROPROOM3D_ARTDIRECTORCLIENT_H
#define PROPROOM3D_ARTDIRECTORCLIENT_H

#include <vector>
#include <thread>

class QTcpSocket;

#include "AbstractArtDirector.h"

#include <PropRoom3D/Node/Node.h>


namespace prop3
{
    class CpuRaytracerEngine;
    class GlPostProdUnit;
    class RaytracerState;
    class DebugRenderer;
    class UpdateMessage;
    class NetworkFilm;


    class PROP3D_EXPORT ArtDirectorClient :
            public AbstractArtDirector
    {
    public:
        ArtDirectorClient();
        virtual ~ArtDirectorClient();

        virtual void setup(const std::shared_ptr<StageSet>& stageSet) override;
        virtual void update(double dt) override;
        virtual void draw(double dt) override;
        virtual void terminate() override;

        virtual void resize(int width, int height) override;
        virtual void notify(cellar::CameraMsg &msg) override;

        std::shared_ptr<RaytracerState> raytracerState() const;

        virtual bool isConnected() const;
        virtual void connectToServer();
        virtual void disconnectFromServer();
        virtual void setServerTcpPort(int port);
        virtual void setServerIpAddress(const std::string& ip);


    protected slots:
        void connected();
        void disconected();
        void readMessage();


    protected:
        virtual void sendBuffersToGpu();
        virtual void sendTilesToServer();


    private:
        QTcpSocket* _socket;
        std::shared_ptr<NetworkFilm> _film;
        std::shared_ptr<StageSet> _stageSet;
        std::unique_ptr<UpdateMessage> _updateMessage;
        std::shared_ptr<GlPostProdUnit> _postProdUnit;
        std::shared_ptr<CpuRaytracerEngine> _localRaytracer;

        bool _isConnected;
        int _serverTcpPort;
        std::string _serverIpAddress;
    };
}

#endif // PROPROOM3D_ARTDIRECTORCLIENT_H
