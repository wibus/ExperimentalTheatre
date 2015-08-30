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
            double distance,
            const glm::dvec3& position,
            const glm::dvec3& incident,
            const glm::dvec3& normal,
            const glm::dvec3& texCoord,
            const Coating* coating);

        void compile();

        double distance;
        glm::dvec3 position;
        glm::dvec3 incident;
        glm::dvec3 normal;
        glm::dvec3 texCoord;
        const Coating* coating;

        // Compiled data
        bool isTextured;
        glm::dvec3 reflectionOrigin;
        glm::dvec3 refractionOrigin;

        // List next node
        RayHitReport* _next;

        static const double EPSILON_LENGTH;
        static const glm::dvec3 NO_TEXCOORD;
    };
}

#endif // PROPROOM3D_RAYHITREPORT_H
