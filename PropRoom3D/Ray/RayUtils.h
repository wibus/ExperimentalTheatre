#ifndef PROPROOM3D_RAYUTILS_H
#define PROPROOM3D_RAYUTILS_H

#include <vector>

#include "Raycast.h"
#include "RayHitReport.h"


namespace prop3
{
    PROP3D_EXPORT glm::dvec3 directDiffuseScattering(
            const Raycast& ray,
            const glm::dvec3& outDir);

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
}

#endif // PROPROOM3D_RAYUTILS_H
