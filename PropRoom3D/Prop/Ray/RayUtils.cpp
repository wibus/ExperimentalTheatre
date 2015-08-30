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
            glm::dot(-report.incident, report.normal)));
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
                    report.incident, report.normal);

            double alpha = (glm::dot(reflectDir, outDir) + 1.0) / 2.0;

            glm::dvec3 diffuse = directDiffuseReflection(report, outDir);
            glm::dvec3 glossy = glm::dvec3(glm::pow(alpha, 1.0 / glossiness));

            return glm::mix(diffuse, glossy, glossiness);
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
                report.incident, report.normal);

        outRays.push_back(Raycast(
                Ray(report.reflectionOrigin, reflectDir),
                glm::dvec3(1.0),
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
                Ray(report.reflectionOrigin, direction),
                glm::dvec3(attenuation),
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
                        report.incident, report.normal);

                glm::dvec3 direction = glm::mix(diffuseDir, reflectDir, glossiness);
                direction = glm::normalize(direction);

                double attenuation = glm::max(0.0, splitFactor *
                        glm::dot(direction, report.normal));

                outRays.push_back(Raycast(
                    Ray(report.reflectionOrigin, direction),
                    glm::dvec3(attenuation),
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
        double reflectionRatio =
                computeReflexionRatio(
                    leavedMaterial->refractiveIndex(),
                    enteredMaterial->refractiveIndex(),
                    report.incident,
                    report.normal);

        glm::dvec3 reflectDir =
                glm::reflect(
                    report.incident,
                    report.normal);

        glm::dvec3 refractDir =
                computeRefraction(
                    leavedMaterial->refractiveIndex(),
                    enteredMaterial->refractiveIndex(),
                    report.incident,
                    report.normal);

        outRays.push_back(Raycast(
                Ray(report.reflectionOrigin, glm::normalize(reflectDir)),
                glm::dvec3(reflectionRatio),
                leavedMaterial));

        outRays.push_back(Raycast(
                Ray(report.refractionOrigin, glm::normalize(refractDir)),
                glm::dvec3(1 - reflectionRatio),
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
