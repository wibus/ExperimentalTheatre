#include "Raycast.h"


namespace prop3
{
   Raycast:: Raycast(
           const Ray& ray,
           const glm::dvec3 color,
           const std::shared_ptr<Material>& material) :
        ray(ray),
        color(color),
        material(material)
    {}
}
