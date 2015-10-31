#include "RayUtils.h"

#include <GLM/gtc/random.hpp>

#include "Prop/Material/Material.h"


namespace prop3
{
    glm::dvec3 directDiffuseScattering(
        const Raycast& ray,
        const glm::dvec3& outDir)
    {
        return ray.color * glm::dvec3(Raycast::compatibility(
            ray.entropy, Raycast::FULLY_DIFFUSIVE_ENTROPY));
    }

    glm::dvec3 directSpecularReflection(
        const RayHitReport& report,
        const glm::dvec3& outDir)
    {
        // Eventhough this is an approximation,
        // floating point precision would not allow
        // to compute the real probability of specular reflexions
        return glm::dvec3(0.0);
    }

    glm::dvec3 directDiffuseReflection(
        const RayHitReport& report,
        const glm::dvec3& outDir)
    {
        double iDotN = glm::dot(report.normal, -report.incidentRay.direction);
        double rDotN = glm::dot(report.normal, outDir);

        if(iDotN < 0.0 || rDotN < 0.0)
            return glm::dvec3(0.0);

        if(rDotN > 1.0)
            rDotN = 1.0;

        return report.incidentRay.color * glm::dvec3(glm::max(0.0, iDotN) *
            Raycast::compatibility(report.incidentRay.entropy,
                                   Raycast::FULLY_DIFFUSIVE_ENTROPY));
    }

    glm::dvec3 directGlossyReflection(
        const RayHitReport& report,
        const glm::dvec3& outDir,
        double glossiness)
    {
        if(glossiness <= 0.0)
        {
            return directDiffuseReflection(report, outDir);
        }
        else if(glossiness >= 1.0)
        {
            return directSpecularReflection(report, outDir);
        }
        else
        {
            double iDotN = glm::dot(report.normal, -report.incidentRay.direction);
            double rDotN = glm::dot(report.normal, outDir);

            if(iDotN > 0.0 || rDotN < 0.0)
                return glm::dvec3(0.0);

            glm::dvec3 reflectDir = glm::reflect(
                    report.incidentRay.direction,
                    report.normal);

            double alpha = (glm::dot(reflectDir, outDir) + 1.0) / 2.0;
            double glossy = glm::pow(alpha, 1.0 / glossiness);

            double diffuse = glm::max(0.0, glm::dot(
                -report.incidentRay.direction, report.normal));

            double compatibility = Raycast::compatibility(
                   report.incidentRay.entropy,
                   Raycast::FULLY_DIFFUSIVE_ENTROPY);

            return report.incidentRay.color * (compatibility *
                    glm::mix(diffuse, glossy, glossiness));
        }
    }

    glm::dvec3 directSpecularRefraction(
        const RayHitReport& report,
        const glm::dvec3& outDir)
    {
        // Eventhough this is an approximation,
        // floating point precision would not allow
        // to compute the real probability of specular refractions
        return glm::dvec3(0.0);
    }
}
