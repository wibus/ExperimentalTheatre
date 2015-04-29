#ifndef PROPROOM3D_CPURAYTRACER_H
#define PROPROOM3D_CPURAYTRACER_H

#include <vector>
#include <thread>

#include <CellarWorkbench/GL/GlProgram.h>

#include "AbstractArtDirector.h"


namespace prop3
{
    class Ray;
    class QGlPostProdUnit;
    class CpuRaytracerWorker;

    class PROP3D_EXPORT CpuRaytracer : public AbstractArtDirector
    {
    public:
        CpuRaytracer();
        CpuRaytracer(unsigned int workerCount);
        virtual ~CpuRaytracer();

        virtual void setup();
        virtual void reset();
        virtual void draw(double dt);

        virtual void notify(cellar::CameraMsg &msg);

        virtual void manageProp(const std::shared_ptr<Prop>& prop);
        virtual void unmanageProp(const std::shared_ptr<Prop>& prop);

    protected:
        virtual void sendBuffersToGpu();

    private:
        void init();

        static const unsigned int DEFAULT_WORKER_COUNT;

        int _workerCount;
        unsigned int _sampleCount;
        unsigned int _colorBufferTexId;

        glm::ivec2 _viewportSize;
        std::vector<float> _colorBuffer;
        std::vector<std::shared_ptr<Prop>> _props;

        friend class CpuRaytracerWorker;
        std::vector<std::thread> _workerThreads;
        std::vector<std::shared_ptr<CpuRaytracerWorker>> _workerObjects;

        std::shared_ptr<QGlPostProdUnit> _postProdUnit;
    };
}

#endif // PROPROOM3D_CPURAYTRACER_H
