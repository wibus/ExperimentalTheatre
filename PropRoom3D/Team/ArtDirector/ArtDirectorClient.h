#ifndef PROPROOM3D_ARTDIRECTORCLIENT_H
#define PROPROOM3D_ARTDIRECTORCLIENT_H

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

        virtual void connectToServer();
        virtual void deconnectFromServer();
        virtual void setServerIpAddress(const std::string& ip);
        virtual void setServerTcpPort(const std::string& port);

        std::shared_ptr<RaytracerState> raytracerState() const;



    protected:
        virtual void sendBuffersToGpu();


    private:
        std::shared_ptr<CpuRaytracerEngine> _localRaytracer;
        std::shared_ptr<GlPostProdUnit> _postProdUnit;
        std::shared_ptr<StageSet> _stageSet;
    };
}

#endif // PROPROOM3D_ARTDIRECTORCLIENT_H
