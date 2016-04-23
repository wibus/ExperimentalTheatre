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
    class ClientSocket;
    class Film;


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

        virtual bool isConnected() const;
        virtual void connectToServer();
        virtual void disconnectFromServer();
        virtual void setServerTcpPort(int port);
        virtual void setServerIpAddress(const std::string& ip);

        std::shared_ptr<RaytracerState> raytracerState() const;


    protected:
        virtual void sendBuffersToGpu();


    private:
        QTcpSocket* _tcpSocket;
        std::shared_ptr<Film> _film;
        std::unique_ptr<ClientSocket> _socket;
        std::shared_ptr<CpuRaytracerEngine> _localRaytracer;
        std::shared_ptr<GlPostProdUnit> _postProdUnit;
        std::shared_ptr<StageSet> _stageSet;

        int _serverTcpPort;
        std::string _serverIpAddress;
    };
}

#endif // PROPROOM3D_ARTDIRECTORCLIENT_H
