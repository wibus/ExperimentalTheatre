#include "StdCoating.h"

#include <GLM/gtc/random.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Ray/RayHitReport.h"
#include "Prop/Material/Material.h"


namespace prop3
{
    // The constant represents the fact that subsurface
    // scattering only returns half of the rays while the other
    // half is scattered further into material's depths...
    //const double INTERNAL_SCATTER_LOSS = 0.61803398876;
    const double INTERNAL_SCATTER_LOSS = 1.0;

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
        glm::dvec3 pColor = glm::dvec3(paintFrag);
        double pOpa = paintFrag.a;


        // Entered material properties
        double lRIdx = currMaterial.refractiveIndex(pos);


        // Paint Reflection
        if(pOpa > 0.0)
        {
            double paintRIdx = paintRefractiveIndex(tex);

            glm::dvec3 diffuseColor = pColor * INTERNAL_SCATTER_LOSS;
            glm::dvec3 reflectColor = color::white;


            glm::dvec3 diffuseNormal = getMicrofacetNormal(
                    wallNormal,
                    incident,
                    1.0); // Fully diffusive

            glm::dvec3 reflectNormal = getMicrofacetNormal(
                    wallNormal,
                    incident,
                    rough);

            glm::dvec3 diffuseDirection =
                    glm::reflect(incident, diffuseNormal);

            glm::dvec3 reflectDirection =
                    glm::reflect(incident, reflectNormal);

            double reflectionRatio =
                    computeReflexionRatio(
                        lRIdx,
                        paintRIdx,
                        incident,
                        reflectNormal);

            double diffuseWeight = (1.0 - reflectionRatio) * pOpa;
            double reflectWeight = reflectionRatio * pOpa;

            glm::dvec4 diffuseSample(diffuseColor * diffuseWeight, diffuseWeight);
            glm::dvec4 reflectSample(reflectColor * reflectWeight, reflectWeight);


            // Diffuse
            raycasts.push_back(Raycast(
                    Raycast::FULLY_DIFFUSE,
                    diffuseSample,
                    reflectOrig,
                    diffuseDirection));

            // Specular
            raycasts.push_back(Raycast(
                    entropy,
                    reflectSample,
                    reflectOrig,
                    reflectDirection));
        }

        // Totally opaque paint
        if(pOpa >= 1.0)
            return emission;


        // Metal reflection
        double eMaterialWeight = (1 - pOpa);
        double eCond = nextMaterial.conductivity(pos);
        glm::dvec3 eColor = nextMaterial.color(pos);
        if(eCond > 0.0)
        {
            double metallicWeight = eMaterialWeight * eCond;
            glm::dvec4 metallicSample(eColor * metallicWeight, metallicWeight);

            glm::dvec3 reflectNormal = getMicrofacetNormal(
                    wallNormal,
                    incident,
                    rough);

            glm::dvec3 reflectDir = glm::reflect(incident, reflectNormal);

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
        double eRIdx = nextMaterial.refractiveIndex(pos);
        double eOpa = nextMaterial.opacity(pos);

        double insulatorWeight = eMaterialWeight * (1 - eCond);


        // Total scattering of the light at the surface
        if(eOpa >= 1.0)
        {
            // This is a shortcut: Since light is almost totally absorbed
            // near the surface, we prevent the ray from entering the
            // material by averaging the effect through a diffuse reflexion.
            double eScat = nextMaterial.scattering(pos);
            double eScatNorm = (1/(1/(1-eScat) - 1) + 1);
            glm::dvec3 diffuseColor = glm::pow(eColor, glm::dvec3(eScatNorm));
            diffuseColor *= INTERNAL_SCATTER_LOSS;
            glm::dvec3 reflectColor = color::white;


            glm::dvec3 diffuseNormal = getMicrofacetNormal(
                    wallNormal,
                    incident,
                    1.0); // Fully diffusive

            glm::dvec3 reflectNormal = getMicrofacetNormal(
                    wallNormal,
                    incident,
                    rough);

            glm::dvec3 diffuseDirection =
                    glm::reflect(incident, diffuseNormal);

            glm::dvec3 reflectDirection =
                    glm::reflect(incident, reflectNormal);

            double reflectionRatio =
                    computeReflexionRatio(
                        lRIdx,
                        eRIdx,
                        incident,
                        reflectNormal);


            double diffuseWeight = (1.0 - reflectionRatio) * insulatorWeight;
            double reflectWeight = reflectionRatio * insulatorWeight;

            glm::dvec4 diffuseSample(diffuseColor * diffuseWeight, diffuseWeight);
            glm::dvec4 reflectSample(reflectColor * reflectWeight, reflectWeight);

            // Reflection
            if(rough < 1.0)
            {
                raycasts.push_back(Raycast(
                        entropy,
                        reflectSample,
                        reflectOrig,
                        reflectDirection));
            }
            else
            {
                diffuseSample += reflectSample;
            }

            // Diffuse
            raycasts.push_back(Raycast(
                    Raycast::FULLY_DIFFUSE,
                    diffuseSample,
                    reflectOrig,
                    diffuseDirection));
        }
        else
        {
            glm::dvec3 refractColor = color::white;
            glm::dvec3 reflectColor = color::white;


            glm::dvec3 reflectNormal = getMicrofacetNormal(
                    wallNormal,
                    incident,
                    rough);

            glm::dvec3 reflectDir = glm::reflect(incident, reflectNormal);

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
            const RayHitReport& report,
            const Raycast& incidentRay,
            const glm::dvec3& outDirection) const
    {
        glm::dvec4 sampleSum = glm::dvec4(0.0);

        // Report's shorthands
        const glm::dvec3& pos = report.position;
        const glm::dvec3& tex = report.texCoord;
        const glm::dvec3& wallNormal = report.normal;
        const glm::dvec3& incident = incidentRay.direction;

        glm::dvec3 reflection = glm::reflect(incident, wallNormal);
        double outDotNorm = glm::dot(outDirection, wallNormal);
        double inDotNorm = -glm::dot(incident, wallNormal);
        bool isTransmission = outDotNorm < 0.0;

        // StdCoating properties
        double rough = roughness(tex);

        // Material properties
        double lRIdx = report.currMaterial->refractiveIndex(pos);
        double eRIdx = report.nextMaterial->refractiveIndex(pos);


        glm::dvec4 paintFrag = paintColor(tex);
        double paintOpa = paintFrag.a;
        if(paintOpa > 0.0 & !isTransmission)
        {
            double paintRIdx = paintRefractiveIndex(tex);
            glm::dvec3 paintCol = glm::dvec3(paintFrag);

            double reflectRatio = computeReflexionRatio(lRIdx, paintRIdx, incident, wallNormal);

            if(rough > 0.0)
            {
                double reflectDotOut = glm::max(0.0, glm::dot(reflection, outDirection));
                double reflectPow = glm::pow(reflectDotOut, 1.0/rough);
                double varnishReflect = glm::mix(reflectPow, inDotNorm, rough);
                glm::dvec4 reflectionSample(varnishReflect * paintOpa * reflectRatio);
                sampleSum += reflectionSample;
            }

            double paintProb = (1.0 - reflectRatio) * inDotNorm * paintOpa;
            glm::dvec4 diffuseSample(paintCol*paintProb, paintProb);
            sampleSum += diffuseSample;
        }

        if(paintOpa >= 1.0)
            return sampleSum;

        if(rough <= 0.0)
            return sampleSum;


        // Metallic reflection
        double eMaterialWeight = (1 - paintOpa);
        double eCond = report.nextMaterial->conductivity(pos);
        glm::dvec3 eColor = report.nextMaterial->color(pos);
        if(eCond > 0.0 && !isTransmission)
        {
            double metalProb = eCond * eMaterialWeight;
            double reflectDotOut = glm::max(0.0, glm::dot(reflection, outDirection));
            double reflectPow = glm::pow(reflectDotOut, 1.0/rough);
            double metallicProb = glm::mix(reflectPow, inDotNorm, rough) * metalProb;
            sampleSum += glm::dvec4(eColor * metallicProb, metallicProb);
        }

        double eOpa = report.nextMaterial->opacity(pos);
        double insulatorWeight = eMaterialWeight * (1 - eCond);
        double reflectRatio = computeReflexionRatio(lRIdx, eRIdx, incident, wallNormal);
        if(isTransmission)
        {
            if(eOpa < 1.0)
            {
                glm::dvec3 refraction = computeRefraction(lRIdx, eRIdx, incident, wallNormal);
                double refractDotOut = glm::max(0.0, glm::dot(refraction, outDirection));
                double refractPow = glm::pow(refractDotOut, 1.0/rough);
                double refractWeight = insulatorWeight * (1.0-reflectRatio);
                glm::dvec4 refractSample(glm::mix(refractPow, -inDotNorm, rough) * refractWeight);
                sampleSum += refractSample;
            }
        }
        else
        {
            double eScat = report.nextMaterial->scattering(pos);
            if(eOpa >= 1.0 && eScat > 0.0)
            {
                double eScatNorm = (1/(1/(1-eScat) - 1) + 1);
                glm::dvec3 diffuseColor = glm::pow(eColor, glm::dvec3(eScatNorm));
                double diffuseWeight = (1.0-reflectRatio) * inDotNorm * insulatorWeight;
                glm::dvec4 diffuseSample(diffuseColor * diffuseWeight, diffuseWeight);
                sampleSum += diffuseSample;
            }

            double reflectWeight = insulatorWeight * reflectRatio;
            double reflectDotOut = glm::max(0.0, glm::dot(reflection, outDirection));
            double reflectPow = glm::pow(reflectDotOut, 1.0/rough);
            double reflectProb = glm::mix(reflectPow, inDotNorm, rough) * reflectWeight;
            sampleSum += glm::dvec4(reflectProb);
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

        glm::dvec3 diffuse = (glm::sphericalRand(1.0) + wallNormal) / 2.0;

        if(rough >= 1.0)
            return glm::normalize(glm::normalize(diffuse) - incidentDir);

        glm::dvec3 specular = glm::reflect(incidentDir, wallNormal);
        glm::dvec3 glossy = glm::mix(specular, diffuse, rough);

        glm::dvec3 normal = glm::normalize(glm::normalize(glossy) - incidentDir);
        return normal;
    }
}
