#ifndef PROPROOM3D_ARTDIRECTORSERVER_H
#define PROPROOM3D_ARTDIRECTORSERVER_H

#include <vector>
#include <thread>

#include "AbstractArtDirector.h"


namespace prop3
{
    class CpuRaytracerEngine;
    class GlPostProdUnit;
    class RaytracerState;
    class DebugRenderer;
    class Film;


    class PROP3D_EXPORT ArtDirectorServer :
            public AbstractArtDirector
    {
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


        static const double FORCE_REFRESH_DT;
        static const double IMAGE_DEPTH;

    protected:
        virtual void sendBuffersToGpu();
        virtual void printConvergence();
        virtual void clearTextures();

    private:
        unsigned int _colorBufferTexId;
        unsigned int _depthBufferTexId;
        std::shared_ptr<CpuRaytracerEngine> _localRaytracer;
        std::shared_ptr<DebugRenderer> _debugRenderer;
        std::shared_ptr<GlPostProdUnit> _postProdUnit;
        std::shared_ptr<StageSet> _stageSet;
    };
}

#endif // PROPROOM3D_ARTDIRECTORSERVER_H
