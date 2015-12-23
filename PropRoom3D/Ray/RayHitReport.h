#ifndef PROPROOM3D_RAYHITREPORT_H
#define PROPROOM3D_RAYHITREPORT_H

#include <memory>

#include "Raycast.h"


namespace prop3
{
    class Coating;


    class PROP3D_EXPORT RayHitReport
    {
	public:
        RayHitReport(
            double length,
            const glm::dvec3& position,
            const glm::dvec3& normal,
            const glm::dvec3& texCoord,
            const Coating* coating,
            const Material* innerMat,
            const Material* outerMat);

        void compile(const glm::dvec3& incident);

        double length;
        glm::dvec3 position;
        glm::dvec3 normal;
        glm::dvec3 texCoord;
        const Coating* coating;
        const Material* innerMat;
        const Material* outerMat;

        // Compiled data
        bool isTextured;
        const Material* nextMaterial;
        const Material* currMaterial;
        glm::dvec3 reflectionOrigin;
        glm::dvec3 refractionOrigin;

        // List next node
        RayHitReport* _next;

        static const double EPSILON_LENGTH;
        static const glm::dvec3 NO_TEXCOORD;
    };
}

#endif // PROPROOM3D_RAYHITREPORT_H
