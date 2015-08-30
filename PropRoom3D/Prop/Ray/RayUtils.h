#ifndef PROPROOM3D_RAYUTILS_H
#define PROPROOM3D_RAYUTILS_H

#include <vector>

#include "Raycast.h"
#include "RayHitReport.h"


namespace prop3
{
    PROP3D_EXPORT glm::dvec3 directSpecularReflection(
            const RayHitReport& report,
            const glm::dvec3& outDir);

    PROP3D_EXPORT glm::dvec3 directDiffuseReflection(
            const RayHitReport& report,
            const glm::dvec3& outDir);

    PROP3D_EXPORT glm::dvec3 directGlossyReflection(
            const RayHitReport& report,
            const glm::dvec3& outDir,
            double glossiness);

    PROP3D_EXPORT glm::dvec3 directSpecularRefraction(
            const RayHitReport& report,
            const glm::dvec3& outDir);


    PROP3D_EXPORT void indirectSpecularReflection(
            std::vector<Raycast>& outRays,
            const RayHitReport& report,
            const std::shared_ptr<Material>& material);

    PROP3D_EXPORT void indirectDiffuseReflection(
            std::vector<Raycast>& outRays,
            const RayHitReport& report,
            const std::shared_ptr<Material>& material,
            unsigned int rayCount);

    PROP3D_EXPORT void indirectGlossyReflection(
            std::vector<Raycast>& outRays,
            const RayHitReport& report,
            const std::shared_ptr<Material>& material,
            double glossiness,
            unsigned int rayCount);

    PROP3D_EXPORT void indirectSpecularRefraction(
            std::vector<Raycast>& outRays,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial);

    PROP3D_EXPORT double computeReflexionRatio(
            double leavedRefractiveIndex,
            double enteredRefractiveIndex,
            const glm::dvec3& incident,
            const glm::dvec3& normal);

    PROP3D_EXPORT glm::dvec3 computeRefraction(
            double leavedRefractiveIndex,
            double enteredRefractiveIndex,
            const glm::dvec3& incident,
            const glm::dvec3& normal);
}

#endif // PROPROOM3D_RAYUTILS_H
