#include "RayHitReport.h"


namespace prop3
{
    RayHitReport::RayHitReport(const Ray& ray,
            double distance,
            const glm::dvec3& position,
            const glm::dvec3& normal,
            const Coating* coating,
            const glm::dvec3& texCoord) :
        ray(ray),
        distance(distance),
        position(position),
        normal(normal),
        coating(coating),
        texCoord(texCoord),
        _next(nullptr)
    {
    }

    void RayHitReport::compile()
    {
        if(glm::dot(ray.direction, normal) > 0.0)
            normal = - normal;

        glm::dvec3 espilonDist = normal * 0.000001;
        reflectionOrigin = position +  espilonDist;
        refractionOrigin = position -  espilonDist;
    }
}
