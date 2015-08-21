#ifndef PROPROOM3D_ARTDIRECTORSERVER_H
#define PROPROOM3D_ARTDIRECTORSERVER_H

#include <vector>
#include <thread>

#include "AbstractArtDirector.h"


namespace prop3
{
    class CpuRaytracerEngine;
    class GlPostProdUnit;

    class PROP3D_EXPORT ArtDirectorServer :
            public AbstractArtDirector
    {
    public:
        ArtDirectorServer();
        virtual ~ArtDirectorServer();

        virtual void setup(const std::shared_ptr<Scene>& scene);
        virtual void reset();
        virtual void draw(double dt);
        virtual void update(double dt);

        virtual void notify(cellar::CameraMsg &msg);

        virtual void manageProp(const std::shared_ptr<Prop>& prop);
        virtual void unmanageProp(const std::shared_ptr<Prop>& prop);

        std::shared_ptr<GlPostProdUnit> postProdUnit() const;


    protected:
        virtual void sendBuffersToGpu();
        virtual void clearColorTexture();

    private:
        unsigned int _colorBufferTexId;
        std::shared_ptr<CpuRaytracerEngine> _localRaytracer;
        std::shared_ptr<GlPostProdUnit> _postProdUnit;
        std::shared_ptr<Scene> _scene;
    };
}

#endif // PROPROOM3D_ARTDIRECTORSERVER_H
