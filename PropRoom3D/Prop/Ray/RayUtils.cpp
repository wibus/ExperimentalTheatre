#include "RayUtils.h"

#include <GLM/gtc/random.hpp>

#include "../Material/Material.h"


namespace prop3
{
    void specularReflection(
        std::vector<Raycast>& outRays,
        const RayHitReport& report,
        const std::shared_ptr<Material>& material)
    {
        glm::dvec3 reflectDir = glm::reflect(
                report.ray.direction, report.normal);

        outRays.push_back(Raycast(
                Ray(report.reflectionOrigin, reflectDir),
                glm::dvec3(1.0),
                material));
    }

    void diffuseReflection(
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

    void glossyReflection(
        std::vector<Raycast>& outRays,
        const RayHitReport& report,
        const std::shared_ptr<Material>& material,
        double glossiness,
        unsigned int rayCount)
    {
        if(glossiness <= 0.0)
        {
            diffuseReflection(outRays, report, material, rayCount);
        }
        else if(glossiness >= 1.0)
        {
            specularReflection(outRays, report, material);
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
                        report.ray.direction, report.normal);

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

    void specularRefraction(
        std::vector<Raycast>& outRays,
        const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial)
    {
        double reflectionRatio =
                computeReflexionRatio(
                    leavedMaterial->refractiveIndex(),
                    enteredMaterial->refractiveIndex(),
                    report.ray.direction,
                    report.normal);

        glm::dvec3 reflectDir =
                glm::reflect(
                    report.ray.direction,
                    report.normal);

        glm::dvec3 refractDir =
                computeRefraction(
                    leavedMaterial->refractiveIndex(),
                    enteredMaterial->refractiveIndex(),
                    report.ray.direction,
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
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal)
    {
        // Schlick's approximation
        double R0 = (leavedRefractiveIndex - enteredRefractiveIndex) /
                    (leavedRefractiveIndex + enteredRefractiveIndex);
        R0 = R0 * R0;

        double cosNV = 1.0 - glm::abs(glm::dot(incidentDirection, surfaceNormal));
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
