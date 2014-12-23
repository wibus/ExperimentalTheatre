#ifndef PROPROOM3D_CPURAYTRACER_H
#define PROPROOM3D_CPURAYTRACER_H

#include <memory>
#include <vector>
#include <thread>

#include <MediaWorkbench/GL/GlProgram.h>

#include "AbstractArtDirector.h"


namespace prop3
{
    class Ray;

    class PROP3D_EXPORT CpuRaytracer : public AbstractArtDirector
    {
    public:
        CpuRaytracer();
        virtual ~CpuRaytracer();

        virtual void setup();
        virtual void reset();
        virtual void draw(double dt);

        virtual void notify(media::CameraMsg &msg);

        virtual void manageProp(const std::shared_ptr<Prop>& prop);
        virtual void unmanageProp(const std::shared_ptr<Prop>& prop);

    protected:
        virtual void resizeBuffers();
        virtual void clearBuffers();
        virtual void shootFromLights();
        virtual void shootFromScreen();
        virtual void sendBuffersToGpu();

        virtual glm::dvec3 fireScreenRay(const Ray& ray, int depth);

    private:

        int _lightRaysBounceCount;
        int _screenRaysBounceCount;

        bool _sceneChanged;
        glm::ivec2 _viewportSize;
        glm::dmat4 _viewMatrix;
        glm::dmat4 _viewProjInverse;
        glm::dvec3 _camPos;

        unsigned int _sampleCount;
        std::vector<float> _colorBuffer;

        unsigned int _framebufferGlId;
        unsigned int _colorBufferGlId;

        media::GlProgram _postProdProgram;
        unsigned int _fullscreenVao;
        unsigned int _fullscreenVbo;

        std::vector<std::shared_ptr<Prop>> _props;

        /* Too hard for the moment
        std::vector<std::thread> _workers;
        friend class CpuRaytracerWorker;
        */
    };
}

#endif // PROPROOM3D_CPURAYTRACER_H
