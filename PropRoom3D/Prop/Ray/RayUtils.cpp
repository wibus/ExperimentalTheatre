#include "RayUtils.h"

#include <GLM/gtc/random.hpp>

#include "../Material/Material.h"


namespace prop3
{
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
        return glm::dvec3(glm::max(0.0,
            glm::dot(-report.incidentRay.direction, report.normal)) *
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

            return glm::dvec3(glm::mix(diffuse, glossy, glossiness) * compatibility);
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


    void indirectSpecularReflection(
        std::vector<Raycast>& outRays,
        const RayHitReport& report,
        const std::shared_ptr<Material>& material)
    {
        glm::dvec3 reflectDir = glm::reflect(
                report.incidentRay.direction, report.normal);

        outRays.push_back(Raycast(
                Raycast::BACKDROP_DISTANCE,
                Raycast::FULLY_SPECULAR_ENTROPY,
                glm::dvec3(1.0),
                report.reflectionOrigin,
                reflectDir,
                material));
    }

    void indirectDiffuseReflection(
        std::vector<Raycast>& outRays,
        const RayHitReport& report,
        const std::shared_ptr<Material>& material,
        unsigned int rayCount)
    {
        double splitFactor(1.0 / rayCount);

        for(int i=0; i<rayCount; ++i)
        {
            glm::dvec3 direction = glm::sphericalRand(1.0);
            if(glm::dot(direction, report.normal) < 0.0)
                direction = -direction;

            double attenuation = glm::max(0.0, splitFactor*
                    glm::dot(direction, report.normal));

            outRays.push_back(Raycast(
                    Raycast::BACKDROP_DISTANCE,
                    Raycast::FULLY_DIFFUSIVE_ENTROPY,
                    glm::dvec3(attenuation),
                    report.reflectionOrigin,
                    direction,
                    material));
        }
    }

    void indirectGlossyReflection(
        std::vector<Raycast>& outRays,
        const RayHitReport& report,
        const std::shared_ptr<Material>& material,
        double glossiness,
        unsigned int rayCount)
    {
        if(glossiness <= 0.0)
        {
            indirectDiffuseReflection(outRays, report, material, rayCount);
        }
        else if(glossiness >= 1.0)
        {
            indirectSpecularReflection(outRays, report, material);
        }
        else
        {
            double splitFactor = 1.0 / rayCount;

            for(int i=0; i<rayCount; ++i)
            {
                glm::dvec3 diffuseDir = glm::sphericalRand(1.0);
                if(glm::dot(diffuseDir, report.normal) < 0.0)
                    diffuseDir = -diffuseDir;

                glm::dvec3 reflectDir = glm::reflect(
                        report.incidentRay.direction, report.normal);

                glm::dvec3 direction = glm::mix(diffuseDir, reflectDir, glossiness);
                direction = glm::normalize(direction);

                glm::dvec3 attenuation = glm::dvec3(glm::max(0.0, splitFactor *
                        glm::dot(direction, report.normal)));

                double entropy = glm::mix(Raycast::FULLY_DIFFUSIVE_ENTROPY,
                                          Raycast::FULLY_SPECULAR_ENTROPY,
                                          glossiness);

                outRays.push_back(Raycast(
                        Raycast::BACKDROP_DISTANCE,
                        entropy,
                        attenuation,
                        report.reflectionOrigin,
                        direction,
                        material));
            }
        }
    }

    void indirectSpecularRefraction(
        std::vector<Raycast>& outRays,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& enteredMaterial)
    {
        const glm::dvec3& incident = report.incidentRay.direction;

        double reflectionRatio =
                computeReflexionRatio(
                    leavedMaterial->refractiveIndex(),
                    enteredMaterial->refractiveIndex(),
                    incident,
                    report.normal);

        glm::dvec3 reflectDir =
                glm::reflect(
                    incident,
                    report.normal);

        glm::dvec3 refractDir =
                computeRefraction(
                    leavedMaterial->refractiveIndex(),
                    enteredMaterial->refractiveIndex(),
                    incident,
                    report.normal);

        outRays.push_back(Raycast(
                Raycast::BACKDROP_DISTANCE,
                Raycast::FULLY_SPECULAR_ENTROPY,
                glm::dvec3(reflectionRatio),
                report.reflectionOrigin,
                glm::normalize(reflectDir),
                leavedMaterial));

        outRays.push_back(Raycast(
                Raycast::BACKDROP_DISTANCE,
                Raycast::FULLY_SPECULAR_ENTROPY,
                glm::dvec3(1 - reflectionRatio),
                report.refractionOrigin,
                glm::normalize(refractDir),
                enteredMaterial));
    }

    double computeReflexionRatio(
            double leavedRefractiveIndex,
            double enteredRefractiveIndex,
            const glm::dvec3& incident,
            const glm::dvec3& normal)
    {
        // Schlick's approximation
        double R0 = (leavedRefractiveIndex - enteredRefractiveIndex) /
                    (leavedRefractiveIndex + enteredRefractiveIndex);
        R0 = R0 * R0;

        double cosNV = 1.0 - glm::abs(glm::dot(incident, normal));
        double cosNV2 = cosNV * cosNV;
        double cosNV5 = cosNV2 * cosNV2 * cosNV;
        return R0 + (1.0 - R0) * cosNV5;
    }

    glm::dvec3 computeRefraction(
            double leavedRefractiveIndex,
            double enteredRefractiveIndex,
            const glm::dvec3& incident,
            const glm::dvec3& normal)
    {
        double eta = leavedRefractiveIndex / enteredRefractiveIndex;
        double cosI = glm::dot(normal, incident);
        double sinT2 = eta * eta * (1.0 - cosI * cosI);

        if(sinT2 <= 1.0)
        {
            double normFact = (eta * cosI + sqrt(1.0 - sinT2));
            return eta * incident - normFact * normal;
        }
        else
        {
            return incident - 2.0 * cosI * normal;
        }
    }
}
