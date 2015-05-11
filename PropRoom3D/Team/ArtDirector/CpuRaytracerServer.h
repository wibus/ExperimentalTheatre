#ifndef PROPROOM3D_CPURAYTRACERSERVER_H
#define PROPROOM3D_CPURAYTRACERSERVER_H

#include <vector>
#include <thread>

#include "AbstractArtDirector.h"


namespace prop3
{
    class CpuRaytracer;
    class GlPostProdUnit;

    class PROP3D_EXPORT CpuRaytracerServer :
            public AbstractArtDirector
    {
    public:
        CpuRaytracerServer();
        virtual ~CpuRaytracerServer();

        virtual void setup();
        virtual void reset();
        virtual void draw(double dt);
        virtual void update(double dt);

        virtual void notify(cellar::CameraMsg &msg);
        virtual void setPostProdManager(
            const std::shared_ptr<prop3::PostProdManager>& manager);

        virtual void manageProp(const std::shared_ptr<Prop>& prop);
        virtual void unmanageProp(const std::shared_ptr<Prop>& prop);


    protected:
        virtual void sendBuffersToGpu();
        virtual void clearColorTexture();

    private:
        unsigned int _colorBufferTexId;
        std::shared_ptr<CpuRaytracer> _localRaytracer;
        std::shared_ptr<GlPostProdUnit> _postProdUnit;
        std::shared_ptr<PostProdManager> _postProdmanager;
    };
}

#endif // PROPROOM3D_CPURAYTRACERSERVER_H
