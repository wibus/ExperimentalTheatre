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


    class PROP3D_EXPORT ArtDirectorServer :
            public AbstractArtDirector
    {
    public:
        ArtDirectorServer();
        virtual ~ArtDirectorServer();

        virtual void setup(const std::shared_ptr<StageSet>& stageSet) override;
        virtual void update(double dt) override;
        virtual void draw(double dt) override;
        virtual void reset() override;

        virtual void notify(cellar::CameraMsg &msg) override;

        std::shared_ptr<GlPostProdUnit> postProdUnit() const;

        std::shared_ptr<RaytracerState> raytracerState() const;


    protected:
        virtual void sendBuffersToGpu();
        virtual void clearColorTexture();

    private:
        unsigned int _colorBufferTexId;
        std::shared_ptr<CpuRaytracerEngine> _localRaytracer;
        std::shared_ptr<GlPostProdUnit> _postProdUnit;
        std::shared_ptr<StageSet> _stageSet;
    };
}

#endif // PROPROOM3D_ARTDIRECTORSERVER_H
