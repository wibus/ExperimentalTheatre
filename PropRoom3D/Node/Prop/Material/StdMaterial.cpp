#include "StdMaterial.h"

#include <CellarWorkbench/Misc/FastMath.h>

#include "Ray/Raycast.h"
#include "Node/Light/LightCast.h"


namespace prop3
{
    StdMaterial::StdMaterial()
    {

    }

    StdMaterial::~StdMaterial()
    {

    }

    double StdMaterial::lightFreePathLength(const Raycast& ray) const
    {
        double opa = opacity(ray.origin);

        if(opa <= 0.0)
        {
            return Raycast::BACKDROP_LIMIT;
        }
        else if(opa >= 1.0)
        {
            return 0.0;
        }
        else
        {
            double scatterRate = 1 / (1 / (opa) - 1);
            std::exponential_distribution<> distrib(scatterRate);
            return distrib(_randomEngine);
        }
    }

    glm::dvec4 StdMaterial::lightAttenuation(const Raycast& ray) const
    {
        double opa = opacity(ray.origin);

        if(opa >= 1.0)
        {
            // Fully pigmented -> no propagation
            return glm::dvec4(0.0);
        }
        else if(opa <= 0.0)
        {
            // Never hits a pigment
            return glm::dvec4(1.0);
        }
        else
        {
            glm::dvec3 col = color(ray.origin);
            double scatterRate = 1 / (1 / opa - 1);
            glm::dvec3 accumulation( ray.limit * scatterRate );
            return glm::dvec4(cellar::fast_pow(col, accumulation), 1.0);
        }
    }

    void StdMaterial::scatterLight(
            std::vector<Raycast>& raycasts,
            const Raycast& ray) const
    {
        double scatt = scattering(ray.origin);
        glm::dvec4 scatterSample(color::white, 1.0);
        glm::dvec3 scatterPoint = ray.origin + ray.direction * ray.limit;

        if(scatt <= 0.0)
        {
            const glm::dvec3& direction = ray.direction;

            raycasts.push_back(Raycast(
                    Raycast::FULLY_SPECULAR,
                    scatterSample,
                    scatterPoint,
                    direction));
        }
        else if(scatt >= 1.0)
        {
            glm::dvec3 direction = _sphereRand.gen(1.0);

            raycasts.push_back(Raycast(
                    Raycast::FULLY_DIFFUSE,
                    scatterSample,
                    scatterPoint,
                    direction));
        }
        else
        {
            glm::dvec3 direction = _sphereRand.gen(1.0);
            direction = glm::mix(ray.direction, direction, scatt);
            direction = glm::normalize(direction);

            raycasts.push_back(Raycast(
                    Raycast::getEntropy(scatt),
                    scatterSample,
                    scatterPoint,
                    direction));
        }
    }

    glm::dvec4 StdMaterial::directBrdf(
        const LightCast& lightCast,
        const Raycast& eyeRay) const
    {
        glm::dvec3 pos = eyeRay.origin + eyeRay.direction * eyeRay.limit;
        double scatt = scattering(pos);

        if(scatt <= 0.0)
            return glm::dvec4(0.0);

        double diffuseLightSize = lightCast.diffuseSize(
            lightCast, eyeRay, Raycast::getEntropy(scatt));

        if(scatt >= 1.0)
        {
            return glm::dvec4(diffuseLightSize);
        }
        else
        {
            double lightDotEye = glm::dot(lightCast.raycast.direction, eyeRay.direction);
            double directness = (1 + lightDotEye) / 2.0;
            double lobe = 1.0 / scatt - 1.0;

            double power = cellar::fast_pow(directness, lobe);
            return glm::dvec4(power * diffuseLightSize);
        }
    }
}
