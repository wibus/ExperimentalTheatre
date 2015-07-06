#ifndef PROPROOM3D_RAYHITREPORT_H
#define PROPROOM3D_RAYHITREPORT_H

#include <memory>

#include "Ray.h"


namespace prop3
{
    class Coating;


    struct PROP3D_EXPORT RayHitReport
    {
        RayHitReport(
            const Ray& ray,
            double distance,
            const glm::dvec3& position,
            const glm::dvec3& normal,
            const Coating* coating,
            const glm::dvec3& texCoord);

        void compile();

        Ray ray;
        double distance;
        glm::dvec3 position;
        glm::dvec3 normal;
        const Coating* coating;
        glm::dvec3 texCoord;

        glm::dvec3 reflectionOrigin;
        glm::dvec3 refractionOrigin;

        // List next node
        RayHitReport* _next;
    };
}

#endif // PROPROOM3D_RAYHITREPORT_H
