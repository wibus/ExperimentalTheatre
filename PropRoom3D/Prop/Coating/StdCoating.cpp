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
        const Material& leavedMaterial,
        const Material& enteredMaterial) const
    {
        // Emission
        glm::dvec4 emission = glm::dvec4(0.0);

        // Report's shorthands
        const glm::dvec3& pos = report.position;
        const glm::dvec3& tex = report.texCoord;
        const glm::dvec3& wallNormal = report.normal;
        const glm::dvec3& incident = report.incidentRay.direction;
        const glm::dvec3& reflectOrig = report.reflectionOrigin;
        const glm::dvec3& refractOrig = report.refractionOrigin;


        // StdCoating properties
        double rough = roughness(tex);

        glm::dvec4 paintFrag = paintColor(tex);
        glm::dvec3 pColor = glm::dvec3(paintFrag);
        double pOpa = paintFrag.a;


        // Entered material properties
        double lRIdx = leavedMaterial.refractiveIndex(pos);


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
                    Raycast::BACKDROP_DISTANCE,
                    diffuseSample,
                    reflectOrig,
                    diffuseDirection));

            // Specular
            raycasts.push_back(Raycast(
                    Raycast::BACKDROP_DISTANCE,
                    reflectSample,
                    reflectOrig,
                    reflectDirection));
        }

        // Totally opaque paint
        if(pOpa >= 1.0)
            return emission;


        // Metal reflection
        double eMaterialWeight = (1 - pOpa);
        double eCond = enteredMaterial.conductivity(pos);
        glm::dvec3 eColor = enteredMaterial.color(pos);
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
                    Raycast::BACKDROP_DISTANCE,
                    metallicSample,
                    reflectOrig,
                    reflectDir));
        }

        // Totally metallic
        if(eCond >= 1.0)
            return emission;


        // Dielectric scattering
        double eRIdx = enteredMaterial.refractiveIndex(pos);
        double eOpa = enteredMaterial.opacity(pos);

        double insulatorWeight = eMaterialWeight * (1 - eCond);


        // Total scattering of the light at the surface
        if(eOpa >= 1.0)
        {
            // This is a shortcut: Since light is almost totally absorbed
            // near the surface, we prevent the ray from entering the
            // material by averaging the effect through a diffuse reflexion.
            double eScat = enteredMaterial.scattering(pos);
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

            // Specular
            if(rough < 1.0)
            {
                raycasts.push_back(Raycast(
                        Raycast::BACKDROP_DISTANCE,
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
                    Raycast::BACKDROP_DISTANCE,
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
                        Raycast::BACKDROP_DISTANCE,
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
                    Raycast::BACKDROP_DISTANCE,
                    reflectSample,
                    reflectOrig,
                    reflectDir));
        }

        // No emission
        return emission;
    }

    glm::dvec4 StdCoating::directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const
    {
        assert(false);
        return glm::dvec4(0.0);
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
