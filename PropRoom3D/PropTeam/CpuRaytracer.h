#ifndef PROPROOM3D_CPURAYTRACER_H
#define PROPROOM3D_CPURAYTRACER_H

#include <memory>
#include <vector>

#include "AbstractArtDirector.h"

namespace prop3
{
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

    private:
        bool _sceneChanged;
        std::vector<std::shared_ptr<Prop>> _props;
    };
}

#endif // PROPROOM3D_CPURAYTRACER_H
