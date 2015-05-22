#ifndef PROPROOM3D_RAYCAST_H
#define PROPROOM3D_RAYCAST_H

#include <memory>

#include "Ray.h"


namespace prop3
{
    class Material;


    struct PROP3D_EXPORT Raycast
    {
        Raycast(const Ray& ray,
                const glm::dvec3 color,
                const std::shared_ptr<Material>& material);

        Ray ray;
        glm::dvec3 color;
        std::shared_ptr<Material> material;
    };
}

#endif // PROPROOM3D_RAYCAST_H
