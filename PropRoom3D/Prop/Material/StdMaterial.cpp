#include "StdMaterial.h"

#include <GLM/gtc/random.hpp>

#include "Ray/Raycast.h"


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
            return ray.limit;
        }
        else if(opa >= 1.0)
        {
            return 0.0;
        }
        else
        {
            double scatterRate = 1 / (1 / (opa) - 1);
            std::exponential_distribution<> distrib(scatterRate);
            double distance = distrib(_randomEngine);
            return glm::min(distance, ray.limit);
        }
    }

    glm::dvec3 StdMaterial::lightAttenuation(const Raycast& ray) const
    {
        double opa = opacity(ray.origin);

        if(opa >= 1.0)
        {
            // Fully pigmented -> no propagation
            return color::black;
        }
        else if(opa <= 0.0)
        {
            // Never hits a pigment
            return color::white;
        }
        else
        {
            glm::dvec3 col = color(ray.origin);
            double scatterRate = 1 / (1 / opa - 1);
            double accumulation = ray.limit * scatterRate;
            return glm::pow(col, glm::dvec3(accumulation));
        }
    }

    void StdMaterial::scatterLight(
            std::vector<Raycast>& raycasts,
            const Raycast& ray) const
    {
        // Ray's shorthands
        const glm::dvec3& orig = ray.origin;

        double scatt = scattering(orig);
        glm::dvec4 scatterSample(color::white, 1.0);
        glm::dvec3 scatterPoint = ray.origin + ray.direction * ray.limit;

        if(scatt >= 1.0)
        {
            glm::dvec3 direction = glm::sphericalRand(1.0);

            raycasts.push_back(Raycast(
                    Raycast::BACKDROP_LIMIT,
                    Raycast::INITIAL_DISTANCE,
                    scatterSample,
                    scatterPoint,
                    direction));
        }
        else if(scatt <= 0.0)
        {
            const glm::dvec3& direction = ray.direction;

            raycasts.push_back(Raycast(
                    Raycast::BACKDROP_LIMIT,
                    Raycast::INITIAL_DISTANCE,
                    scatterSample,
                    scatterPoint,
                    direction));
        }
        else
        {
            glm::dvec3 diffuseDir = glm::sphericalRand(1.0);
            glm::dvec3 direction = glm::mix(ray.direction, diffuseDir, scatt);
            direction = glm::normalize(direction);

            raycasts.push_back(Raycast(
                    Raycast::BACKDROP_LIMIT,
                    Raycast::INITIAL_DISTANCE,
                    scatterSample,
                    scatterPoint,
                    direction));
        }
    }
}
