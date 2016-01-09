#include "StdCoating.h"

#include <GLM/gtc/random.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Light/LightCast.h"
#include "Ray/RayHitReport.h"
#include "Prop/Material/Material.h"


namespace prop3
{
    StdCoating::StdCoating()
    {

    }

    StdCoating::~StdCoating()
    {

    }

    glm::dvec4 StdCoating::indirectBrdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const Raycast& incidentRay) const
    {
        // Emission
        glm::dvec4 emission = glm::dvec4(0.0);

        // Report's shorthands
        const glm::dvec3& pos = report.position;
        const glm::dvec3& tex = report.texCoord;
        const glm::dvec3& wallNormal = report.normal;
        const glm::dvec3& reflectOrig = report.reflectionOrigin;
        const glm::dvec3& refractOrig = report.refractionOrigin;
        const Material& currMaterial = *report.currMaterial;
        const Material& nextMaterial = *report.nextMaterial;
        const glm::dvec3& incident = incidentRay.direction;

        // StdCoating properties
        double rough = roughness(tex);
        double entropy = Raycast::getEntropy(rough);
        glm::dvec4 paintFrag = paintColor(tex);
        double pOpa = paintFrag.a;

        // Leaved material properties
        double lRIdx = currMaterial.refractiveIndex(pos);

        // Entered material properties
        double eOpa = nextMaterial.opacity(pos);
        double eCond = nextMaterial.conductivity(pos);
        double eRIdx = nextMaterial.refractiveIndex(pos);
        glm::dvec3 eColor = nextMaterial.color(pos);

        // Reflection
        glm::dvec3 reflectNormal = getMicrofacetNormal(
                wallNormal, incident, rough);
        glm::dvec3 reflectDir = glm::reflect(
                incident, reflectNormal);


        // Surface reflection (paint and insulator)
        if(pOpa > 0.0 || (eOpa >= 1.0 && eCond < 1.0))
        {
            double totalDiffScatt = (eOpa >= 1.0 ? 1.0 : 0.0);
            double blend = (1.0 - pOpa) * totalDiffScatt;

            double pRIdx = paintRefractiveIndex(tex);
            double sRIdx = glm::mix(pRIdx, eRIdx, blend);

            double reflectRatio = computeReflexionRatio(
                lRIdx, sRIdx, incident, reflectNormal);

            double surfaceWeight = pOpa + (1 - pOpa) * (1 - eCond) * totalDiffScatt;
            double diffuseWeight = (1.0 - reflectRatio) * surfaceWeight;
            double reflectWeight = reflectRatio * surfaceWeight;

            glm::dvec3 pColor = glm::dvec3(paintFrag);
            glm::dvec3 diffuseColor = glm::mix(pColor, eColor, blend);
            const glm::dvec3& reflectColor = color::white;

            glm::dvec4 diffuseSample(diffuseColor * diffuseWeight, diffuseWeight);
            glm::dvec4 reflectSample(reflectColor * reflectWeight, reflectWeight);


            // Diffuse
            if(rough < 1.0)
            {
                glm::dvec3 diffuseNormal = getMicrofacetNormal(
                        wallNormal, incident, 1.0); // Fully diffusive

                glm::dvec3 diffuseDir = glm::reflect(
                        incident, diffuseNormal);

                raycasts.push_back(Raycast(
                        Raycast::FULLY_DIFFUSE,
                        diffuseSample,
                        reflectOrig,
                        diffuseDir));
            }
            else
            {
                reflectSample += diffuseSample;
            }

            // Specular
            raycasts.push_back(Raycast(
                    entropy,
                    reflectSample,
                    reflectOrig,
                    reflectDir));
        }

        // Totally opaque diffuse surface
        if(pOpa >= 1.0 || (eOpa >= 1.0 && eCond <= 0.0))
            return emission;


        // Metal reflection
        double eMaterialWeight = (1 - pOpa);
        if(eCond > 0.0)
        {
            double metallicWeight = eMaterialWeight * eCond;
            glm::dvec4 metallicSample(eColor * metallicWeight, metallicWeight);

            // Metallic
            raycasts.push_back(Raycast(
                    entropy,
                    metallicSample,
                    reflectOrig,
                    reflectDir));
        }

        // Totally metallic
        if(eCond >= 1.0)
            return emission;


        // Dielectric scattering
        double insulatorWeight = eMaterialWeight * (1 - eCond);
        if(eOpa < 1.0)
        {
            const glm::dvec3& refractColor = color::white;
            const glm::dvec3& reflectColor = color::white;

            glm::dvec3 refractDir =
                    computeRefraction(
                        lRIdx,
                        eRIdx,
                        incident,
                        reflectNormal);


            double reflectionRatio =
                    computeReflexionRatio(
                        lRIdx,
                        eRIdx,
                        incident,
                        reflectNormal);

            double refractWeight = (1.0 - reflectionRatio) * insulatorWeight;
            double reflectWeight = reflectionRatio * insulatorWeight;

            glm::dvec4 refractSample(refractColor * refractWeight, refractWeight);
            glm::dvec4 reflectSample(reflectColor * reflectWeight, reflectWeight);


            // Refraction
            if(glm::dot(refractDir, wallNormal) < 0.0)
            {
                raycasts.push_back(Raycast(
                        entropy,
                        refractSample,
                        refractOrig,
                        refractDir));
            }
            else
            {
                reflectSample += refractSample;
            }

            // Reflexion
            raycasts.push_back(Raycast(
                    entropy,
                    reflectSample,
                    reflectOrig,
                    reflectDir));
        }

        // No emission
        return emission;
    }

    glm::dvec4 StdCoating::directBrdf(
            const LightCast& lightCast,
            const RayHitReport& report,
            const Raycast& eyeRay) const
    {
        glm::dvec4 sampleSum = glm::dvec4(0.0);

        // Report's shorthands
        const glm::dvec3& pos = report.position;
        const glm::dvec3& tex = report.texCoord;
        const glm::dvec3& wallNormal = report.normal;
        const glm::dvec3& incident = lightCast.raycast.direction;

        // StdCoating properties
        double rough = roughness(tex);
        double pRIdx = paintRefractiveIndex(tex);
        glm::dvec4 paintFrag = paintColor(tex);
        double pOpa = paintFrag.a;

        // Material properties
        double lRIdx = report.currMaterial->refractiveIndex(pos);
        double eRIdx = report.nextMaterial->refractiveIndex(pos);
        double eCond = report.nextMaterial->conductivity(pos);
        double eOpa = report.nextMaterial->opacity(pos);

        // Geometry
        glm::dvec3 outDir = -eyeRay.direction;
        double inDotNorm = -glm::dot(incident, wallNormal);
        double outDotNorm = glm::dot(outDir, wallNormal);
        bool isTransmission = outDotNorm < 0.0;

        // Fresnel reflection
        double paintReflectRatio = computeReflexionRatio(
            lRIdx, pRIdx, incident, wallNormal);
        double matReflectRatio = computeReflexionRatio(
            lRIdx, eRIdx, incident, wallNormal);


        if(!isTransmission)
        {
            glm::dvec3 eColor = report.nextMaterial->color(pos);
            double diffuseLightSize = lightCast.diffuseSize(
                lightCast, eyeRay, Raycast::FULLY_DIFFUSE);

            double totalDiffScatt = (eOpa >= 1.0 ? 1.0 : 0.0);

            double metalProb = (1 - pOpa) * eCond;
            double insulProb = (1 - pOpa) * (1 - eCond);
            double matDiffProb = insulProb * totalDiffScatt * (1 - matReflectRatio);
            double diffuseProb = pOpa * (1 - paintReflectRatio) + matDiffProb;
            double fresnelProb = pOpa * paintReflectRatio + insulProb * matReflectRatio;
            double reflectProb = fresnelProb + metalProb;

            double diffuseIntens = inDotNorm * diffuseLightSize;

            if(diffuseProb > 0.0)
            {
                double diffuseWeight = diffuseProb * diffuseIntens;

                glm::dvec3 pColor = glm::dvec3(paintFrag);
                glm::dvec3 diffuseColor = glm::mix(pColor, eColor, matDiffProb / diffuseProb);
                glm::dvec4 diffSample(diffuseColor * diffuseWeight, diffuseWeight);
                sampleSum += diffSample;
            }

            if(reflectProb > 0.0 && rough > 0.0)
            {
                double reflectLightSize = lightCast.diffuseSize(
                    lightCast, eyeRay, rough);

                glm::dvec3 reflection = glm::reflect(incident, wallNormal);
                double outDotReflect = glm::max(glm::dot(outDir, reflection), 0.0);
                double reflectPower = glm::pow(outDotReflect, 1 / rough);
                double reflectIntens = reflectPower * reflectLightSize;
                reflectIntens = glm::mix(reflectIntens, diffuseIntens, rough);
                double reflectWeight = reflectProb * reflectIntens;

                glm::dvec3 reflectColor = glm::mix(color::white, eColor, metalProb / reflectProb);
                glm::dvec4 reflectSample(reflectColor * reflectWeight, reflectWeight);
                sampleSum += reflectSample;
            }
        }
        else if(rough > 0.0 && eOpa < 1.0)
        {
            double insulProb = (1 - pOpa) * (1 - eCond);

            if(insulProb > 0.0)
            {
                double reflectLightSize = lightCast.diffuseSize(
                    lightCast, eyeRay, rough);

                glm::dvec3 refraction = computeRefraction(lRIdx, eRIdx, incident, wallNormal);
                double refractDotOut = glm::max(glm::dot(refraction, outDir), 0.0);
                double refractPower = glm::pow(refractDotOut, 1 / rough);
                refractPower = glm::mix(refractPower, inDotNorm, rough);

                double paintRefract = pOpa * (1 - paintReflectRatio);
                double insulRefract = insulProb * (1 - matReflectRatio);
                double refractProb = (paintRefract + insulRefract) * refractPower * reflectLightSize;

                glm::dvec4 refractSample(refractProb);
                sampleSum += refractSample;
            }
        }

        return sampleSum;
    }

    glm::dvec3 StdCoating::albedo(const RayHitReport& report) const
    {
        glm::dvec4 paint = paintColor(report.texCoord);
        glm::dvec3 matCol = report.nextMaterial->color(report.position);
        return glm::mix(matCol, glm::dvec3(paint), paint.a);
    }

    inline glm::dvec3 StdCoating::getMicrofacetNormal(
            const glm::dvec3& wallNormal,
            const glm::dvec3& incidentDir,
            double rough)
    {
        if(rough <= 0.0)
            return wallNormal;

        glm::dvec2 randV = glm::linearRand(glm::dvec2(0), glm::dvec2(1.0));

        double phi = 2 * glm::pi<double>() * randV.x;
        double zee = glm::pow(randV.y, 1.0 / (1.0/rough + 1.0));
        double rad = glm::sqrt(1.0 - zee*zee);
        double csp = glm::cos(phi) * rad;
        double snp = glm::sin(phi) * rad;

        glm::dvec3 nonco = glm::abs(wallNormal.x) < glm::abs(wallNormal.z) ?
            glm::dvec3(1, 0, 0) : glm::dvec3(0, 0, 1);
        glm::dvec3 tangent = glm::normalize(glm::cross(wallNormal, nonco));
        glm::dvec3 binorm = glm::normalize(glm::cross(wallNormal, tangent));

        glm::dvec3 specular = glm::reflect(incidentDir, wallNormal);
        glm::dvec3 center = glm::mix(specular, wallNormal, rough);
        glm::dvec3 diffuse = center*zee + tangent*csp + binorm*snp;

        glm::dvec3 normal = glm::normalize(glm::normalize(diffuse) - incidentDir);
        return normal;
    }
}
