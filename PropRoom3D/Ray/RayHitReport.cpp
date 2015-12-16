#include "RayHitReport.h"


namespace prop3
{
    const double RayHitReport::EPSILON_LENGTH = 0.000001;
    const glm::dvec3 RayHitReport::NO_TEXCOORD = glm::dvec3(INFINITY);

    RayHitReport::RayHitReport(
            double length,
            const Raycast& incidentRay,
            const glm::dvec3& position,
            const glm::dvec3& normal,
            const glm::dvec3& texCoord,
            const Coating* coating,
            const Material* innerMat,
            const Material* outerMat) :
        length(length),
        incidentRay(incidentRay),
        position(position),
        normal(normal),
        texCoord(texCoord),
        coating(coating),
        innerMat(innerMat),
        outerMat(outerMat),
        nextMaterial(nullptr),
        currMaterial(nullptr),
        _next(nullptr)
    {
    }

    void RayHitReport::compile()
    {
        isTextured = (texCoord != NO_TEXCOORD);

        if(glm::dot(incidentRay.direction, normal) > 0.0)
        {
            normal = -normal;
            nextMaterial = outerMat;
            currMaterial = innerMat;
        }
        else
        {
            nextMaterial = innerMat;
            currMaterial = outerMat;
        }

        glm::dvec3 espilonDist = normal * EPSILON_LENGTH;
        reflectionOrigin = position +  espilonDist;
        refractionOrigin = position -  espilonDist;
    }
}
