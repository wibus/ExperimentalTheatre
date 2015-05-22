#ifndef PROPROOM3D_RAYUTILS_H
#define PROPROOM3D_RAYUTILS_H

#include <vector>

#include "Raycast.h"
#include "RayHitReport.h"


namespace prop3
{
    PROP3D_EXPORT void specularReflection(
            std::vector<Raycast>& outRays,
            const RayHitReport& report,
            const std::shared_ptr<Material>& material);

    PROP3D_EXPORT void diffuseReflection(
            std::vector<Raycast>& outRays,
            const RayHitReport& report,
            const std::shared_ptr<Material>& material,
            unsigned int rayCount);

    PROP3D_EXPORT void glossyReflection(
            std::vector<Raycast>& outRays,
            const RayHitReport& report,
            const std::shared_ptr<Material>& material,
            double glossiness,
            unsigned int rayCount);

    PROP3D_EXPORT void specularRefraction(
            std::vector<Raycast>& outRays,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial);

    PROP3D_EXPORT double computeReflexionRatio(
            double leavedRefractiveIndex,
            double enteredRefractiveIndex,
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal);

    PROP3D_EXPORT glm::dvec3 computeRefraction(
            double leavedRefractiveIndex,
            double enteredRefractiveIndex,
            const glm::dvec3& incident,
            const glm::dvec3& normal);
}

#endif // PROPROOM3D_RAYUTILS_H
