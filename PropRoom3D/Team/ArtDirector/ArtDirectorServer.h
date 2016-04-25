#ifndef PROPROOM3D_ARTDIRECTORSERVER_H
#define PROPROOM3D_ARTDIRECTORSERVER_H

#include <list>
#include <vector>
#include <thread>

#include "AbstractArtDirector.h"
#include <PropRoom3D/Node/Node.h>


namespace prop3
{
    class CpuRaytracerEngine;
    class GlPostProdUnit;
    class RaytracerState;
    class DebugRenderer;
    class UpdateMessage;
    class TcpServer;

    class Film;
    class ConvergentFilm;


    class PROP3D_EXPORT ArtDirectorServer :
            public AbstractArtDirector
    {
        Q_OBJECT

    public:
        ArtDirectorServer();
        virtual ~ArtDirectorServer();

        virtual void setup(const std::shared_ptr<StageSet>& stageSet) override;
        virtual void update(double dt) override;
        virtual void draw(double dt) override;
        virtual void terminate() override;

        virtual void resize(int width, int height) override;
        virtual void notify(cellar::CameraMsg &msg) override;

        std::shared_ptr<GlPostProdUnit> postProdUnit() const;
        std::shared_ptr<RaytracerState> raytracerState() const;
        std::shared_ptr<Film> film() const;

        std::string ipAddress() const;

        int tcpPort() const;
        void setTcpPort(int port);

        bool isRunning() const;
        void turnOn();
        void turnOff();

        static const double IMAGE_DEPTH;
        static const int DEFAULT_TCP_PORT;

    protected:
        virtual void sendBuffersToGpu();
        virtual void printConvergence();

    protected:
        void shotChanged();

    private:
        int _tcpPort;
        TcpServer* _tcpServer;
        bool _rebuildUpdateMsg;
        bool _shotIsStable;

        std::shared_ptr<ConvergentFilm> _film;
        std::shared_ptr<CpuRaytracerEngine> _localRaytracer;
        std::shared_ptr<DebugRenderer> _debugRenderer;
        std::shared_ptr<GlPostProdUnit> _postProdUnit;
        std::shared_ptr<StageSet> _stageSet;
        std::string _stageSetStream;
        TimeStamp _lastUpdate;
    };
}

#endif // PROPROOM3D_ARTDIRECTORSERVER_H
