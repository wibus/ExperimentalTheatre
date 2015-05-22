#include "Chrome.h"

#include "../Ray/Raycast.h"
#include "../Ray/RayUtils.h"


namespace prop3
{
    Chrome::Chrome(const glm::dvec3& color) :
        _color(color)
    {
    }

    Chrome::~Chrome()
    {

    }

    void Chrome::brdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& selfEnteredMaterial,
        unsigned int outRayCountHint) const
    {
        size_t preSize = raycasts.size();
        specularReflection(raycasts, report, selfEnteredMaterial);
        size_t postSize = raycasts.size();

        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= _color;
        }
    }
}
