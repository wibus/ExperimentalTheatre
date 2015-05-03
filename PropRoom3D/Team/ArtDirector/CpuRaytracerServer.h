#ifndef PROPROOM3D_CPURAYTRACERSERVER_H
#define PROPROOM3D_CPURAYTRACERSERVER_H

#include <vector>
#include <thread>

#include "AbstractArtDirector.h"


namespace prop3
{
    class CpuRaytracer;
    class QGlPostProdUnit;

    class PROP3D_EXPORT CpuRaytracerServer :
            public AbstractArtDirector
    {
    public:
        CpuRaytracerServer();
        virtual ~CpuRaytracerServer();

        virtual void setup();
        virtual void reset();
        virtual void draw(double dt);

        virtual void notify(cellar::CameraMsg &msg);

        virtual void manageProp(const std::shared_ptr<Prop>& prop);
        virtual void unmanageProp(const std::shared_ptr<Prop>& prop);

    protected:
        virtual void sendBuffersToGpu();
        virtual void clearColorTexture();

    private:
        unsigned int _colorBufferTexId;
        std::shared_ptr<CpuRaytracer> _localRaytracer;
        std::shared_ptr<QGlPostProdUnit> _postProdUnit;
    };
}

#endif // PROPROOM3D_CPURAYTRACERSERVER_H
