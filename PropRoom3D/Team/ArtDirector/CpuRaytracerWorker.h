#ifndef PROPROOM3D_CPURAYTRACERWORKER_H
#define PROPROOM3D_CPURAYTRACERWORKER_H

#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>

#include <GLM/glm.hpp>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    class Ray;
    class Prop;

    class PROP3D_EXPORT CpuRaytracerWorker
    {
    public:
        static void launchWorker(
            const std::shared_ptr<CpuRaytracerWorker>& worker);

        CpuRaytracerWorker();

        virtual void nextFrame();
        virtual void skip();
        virtual void terminate();

        virtual void resize(int width, int height);
        virtual void setProps(const std::vector<std::shared_ptr<Prop>>& props);

        virtual bool tryLockPixels();
        virtual bool frameIsComplete();
        virtual const std::vector<float>& pixels() const;
        virtual void unlockPixels();

    protected:
        virtual void execute();
        virtual void resizeBuffers();
        virtual void shootFromLights();
        virtual void shootFromScreen();
        virtual glm::dvec3 fireScreenRay(
                const Ray& ray,
                int depth);

    private:
        std::mutex _skipMutex;
        bool _skipPredicate;
        std::mutex _flowMutex;
        bool _readyPredicate;
        std::mutex _terminateMutex;
        bool _terminatePredicate;
        std::condition_variable _cv;


        int _lightRaysBounceCount;
        int _screenRaysBounceCount;

        glm::ivec2 _viewportSize;
        glm::dmat4 _viewMatrix;
        glm::dmat4 _viewProjInverse;
        glm::dvec3 _camPos;

        bool _frameIsComplete;
        std::vector<float> _colorBuffer;

        std::vector<std::shared_ptr<Prop>> _props;
    };
}

#endif // PROPROOM3D_CPURAYTRACERWORKER_H
