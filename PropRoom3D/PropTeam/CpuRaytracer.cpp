#include "CpuRaytracer.h"
#include "Prop/Volume/Raycast.h"

#include <algorithm>


namespace prop3
{
    CpuRaytracer::CpuRaytracer() :
        _sceneChanged(true)
    {

    }

    CpuRaytracer::~CpuRaytracer()
    {

    }

    void CpuRaytracer::setup()
    {

    }

    void CpuRaytracer::reset()
    {
        _props.clear();
    }

    void CpuRaytracer::draw(double dt)
    {

    }

    void CpuRaytracer::notify(media::CameraMsg &msg)
    {
        _sceneChanged = true;
    }

    void CpuRaytracer::manageProp(const std::shared_ptr<Prop>& prop)
    {
        _props.push_back(prop);
    }

    void CpuRaytracer::unmanageProp(const std::shared_ptr<Prop>& prop)
    {
        std::remove_if(_props.begin(), _props.end(),
            [&prop](const std::shared_ptr<Prop>& p) {
                return p == prop;
        });
    }
}
