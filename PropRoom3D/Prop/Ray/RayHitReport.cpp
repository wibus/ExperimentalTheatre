#include "RayHitReport.h"


namespace prop3
{
    const double RayHitReport::EPSILON_LENGTH = 0.000001;
    const glm::dvec3 RayHitReport::NO_TEXCOORD = glm::dvec3(INFINITY);

    RayHitReport::RayHitReport(
            double distance,
            const glm::dvec3& position,
            const glm::dvec3& incident,
            const glm::dvec3& normal,
            const glm::dvec3& texCoord,
            const Coating* coating) :
        distance(distance),
        position(position),
        incident(incident),
        normal(normal),
        texCoord(texCoord),
        coating(coating),
        _next(nullptr)
    {
    }

    void RayHitReport::compile()
    {
        isTextured = (texCoord != NO_TEXCOORD);

        if(glm::dot(incident, normal) > 0.0)
            normal = - normal;

        glm::dvec3 espilonDist = normal * EPSILON_LENGTH;
        reflectionOrigin = position +  espilonDist;
        refractionOrigin = position -  espilonDist;
    }
}
