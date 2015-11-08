#include "StdCoating.h"

#include <GLM/gtc/random.hpp>
#include <GLM/gtc/matrix_transform.hpp>

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

    void StdCoating::indirectBrdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const Material& leavedMaterial,
        const Material& enteredMaterial,
        unsigned int outRayCountHint) const
    {
        // Report's shorthands
        const glm::dvec3& pos = report.position;
        const glm::dvec3& tex = report.texCoord;
        const glm::dvec3& wallNormal = report.normal;
        const glm::dvec3& incident = report.incidentRay.direction;
        const glm::dvec3& reflectOrig = report.reflectionOrigin;
        const glm::dvec3& refractOrig = report.refractionOrigin;

        // StdCoating properties
        double rough = roughness(tex);
        double paintRIdx = paintRefractiveIndex(tex);
        glm::dvec4 paintFrag = paintColor(tex);
        glm::dvec3 pColor = glm::dvec3(paintFrag);
        double pOpa = paintFrag.a;
        int rayCount = glm::round((outRayCountHint-1) * rough) + 1;
        double normalEntropy = glm::mix(
                Raycast::FULLY_SPECULAR_ENTROPY,
                Raycast::FULLY_DIFFUSIVE_ENTROPY,
                rough);

        // Entered material properties
        double lRIdx = leavedMaterial.refractiveIndex(pos);
        double eCond = enteredMaterial.conductivity(pos);
        glm::dvec3 eColor = enteredMaterial.color(pos);


        // Paint Reflection
        if(pOpa > 0.0)
        {
            glm::dvec3 reflectColor = glm::dvec3(pOpa / rayCount);
            glm::dvec3 diffuseColor = pColor / double(rayCount);

            for(int r=0; r < rayCount; ++r)
            {
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

                // The constant represents the fact that subsurface
                // scattering only returns half of the rays while the other
                // half is scattered further into material's depths...
                const double CONST = 0.61803398876;
                glm::dvec3 attenuation = diffuseColor * (1 - reflectionRatio)
                       * (glm::dot(diffuseDirection, wallNormal)) * CONST;


                raycasts.push_back(Raycast(
                        Raycast::BACKDROP_DISTANCE,
                        Raycast::FULLY_DIFFUSIVE_ENTROPY,
                        attenuation,
                        reflectOrig,
                        diffuseDirection));

                raycasts.push_back(Raycast(
                        Raycast::BACKDROP_DISTANCE,
                        normalEntropy,
                        reflectColor * reflectionRatio,
                        reflectOrig,
                        reflectDirection));
            }
        }

        if(pOpa >= 1.0)
            return;


        // Metal reflection
        if(eCond > 0.0)
        {
            glm::dvec3 metallicColor = eColor * ((1 - pOpa) *eCond / rayCount);

            for(int r=0; r < rayCount; ++r)
            {
                glm::dvec3 reflectNormal = getMicrofacetNormal(
                        wallNormal,
                        incident,
                        rough);

                glm::dvec3 reflectDir = glm::reflect(incident, reflectNormal);

                raycasts.push_back(Raycast(
                        Raycast::BACKDROP_DISTANCE,
                        normalEntropy,
                        metallicColor,
                        reflectOrig,
                        reflectDir));
            }
        }


        // Dielectric scattering
        if(eCond < 1.0)
        {
            double eRIdx = enteredMaterial.refractiveIndex(pos);
            double eOpa = enteredMaterial.opacity(pos);

            glm::dvec3 insulatorColor = glm::dvec3(
                (1 - pOpa) * (1 - eCond) / rayCount);


            // Total scattering of the light at the surface
            if(eOpa >= 1.0)
            {
                // This is a shortcut: Since light is almost totally absorbed
                // near the surface, we prevent the ray from entering the
                // material by averaging the effect by a diffuse reflexion.
                double eScat = enteredMaterial.scattering(pos);
                glm::dvec3 diffuseColor = glm::mix(color::black,
                        glm::pow(eColor, glm::dvec3(1/(1/(1-eScat) - 1) + 1)), eScat)
                    * ((1.0 - eCond) / outRayCountHint);

                // The constant represents the fact that subsurface
                // scattering only returns half of the rays while the other
                // half is scattered further into material's depths...
                const double CONST = 0.61803398876;


                for(int r=0; r < outRayCountHint; ++r)
                {
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


                    glm::dvec3 attenuation = diffuseColor * (1 - reflectionRatio)
                           * (glm::dot(diffuseDirection, wallNormal) * CONST);


                    raycasts.push_back(Raycast(
                            Raycast::BACKDROP_DISTANCE,
                            Raycast::FULLY_DIFFUSIVE_ENTROPY,
                            attenuation,
                            reflectOrig,
                            diffuseDirection));

                    raycasts.push_back(Raycast(
                            Raycast::BACKDROP_DISTANCE,
                            normalEntropy,
                            insulatorColor * reflectionRatio,
                            reflectOrig,
                            reflectDirection));
                }
            }
            else
            {
                glm::dvec3 refractColor = insulatorColor *
                        glm::mix(color::white, pColor, pOpa);

                for(int r=0; r < rayCount; ++r)
                {
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

                    glm::dvec3 currRefractOrig = refractOrig;
                    if(glm::dot(refractDir, wallNormal) > 0.0)
                        currRefractOrig = reflectOrig;

                    double reflectionRatio =
                            computeReflexionRatio(
                                lRIdx,
                                eRIdx,
                                incident,
                                reflectNormal);


                    raycasts.push_back(Raycast(
                            Raycast::BACKDROP_DISTANCE,
                            normalEntropy,
                            insulatorColor * reflectionRatio,
                            reflectOrig,
                            reflectDir));

                    raycasts.push_back(Raycast(
                            Raycast::BACKDROP_DISTANCE,
                            normalEntropy,
                            refractColor * (1 - reflectionRatio),
                            currRefractOrig,
                            refractDir));
                }
            }
        }
    }

    glm::dvec3 StdCoating::directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const
    {
        assert(false);
    }

    inline glm::dvec3 StdCoating::getMicrofacetNormal(
            const glm::dvec3& wallNormal,
            const glm::dvec3& incidentDir,
            double rough)
    {
        if(rough <= 0.0)
            return wallNormal;

        glm::dvec3 diffuse = glm::sphericalRand(1.0);
        if(glm::dot(diffuse, wallNormal) < 0.0)
            diffuse = -diffuse;

        if(rough >= 1.0)
            return glm::normalize((diffuse - incidentDir) / 2.0);

        glm::dvec3 specular = glm::reflect(incidentDir, wallNormal);
        glm::dvec3 glossy = glm::mix(specular, diffuse, rough);

        glm::dvec3 normal = glm::normalize((glossy - incidentDir) / 2.0);
        return normal;
    }
}
