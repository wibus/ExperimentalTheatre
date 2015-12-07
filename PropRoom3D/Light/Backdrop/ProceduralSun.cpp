#include "ProceduralSun.h"

#include <GLM/gtc/random.hpp>
#include <GLM/gtc/constants.hpp>

#include "Prop/Material/Material.h"
#include "Ray/Raycast.h"
#include "Ray/RayHitList.h"
#include "Node/Visitor.h"
#include "Light/Light.h"


namespace prop3
{
    const double ProceduralSun::BACKDROP_DISTANCE = 20.0;
    const double ProceduralSun::SUN_COS_DIMENSION = 0.99996192306;
    const double ProceduralSun::SUN_SMOOTH_EDGE = 5.0e4;
    const glm::dvec3 ProceduralSun::SKY_UP = glm::dvec3(0, 0, 1);

    const double ProceduralSun::SUN_SIN =
        glm::sqrt(1.0 - ProceduralSun::SUN_COS_DIMENSION * ProceduralSun::SUN_COS_DIMENSION);
    const glm::dvec3 ProceduralSun::RECEIVE_PLANE_POS =
        glm::dvec3(0.0, 0.0, 0.0);
    const double ProceduralSun::RECEIVE_PLANE_RADIUS = 30.0;

    const double ProceduralSun::RADIATION_PLANE_DISTANCE = 30.0;
    const double ProceduralSun::RADIATION_PLANE_RADIUS =
            ProceduralSun::RADIATION_PLANE_DISTANCE *
            ProceduralSun::SUN_SIN;

    ProceduralSun::ProceduralSun(bool isDirectlyVisible) :
        Backdrop(isDirectlyVisible),
        _sunColor(glm::dvec3(1.00, 0.75, 0.62) * 1e5),
        _skyColor(glm::dvec3(0.25, 0.60, 1.00) * 2.0),
        _skylineColor(glm::dvec3(1.00, 1.00, 1.00) * 2.0),
        _groundColor(glm::dvec3(0.05, 0.05, 0.3)),
        _groundHeight(-0.2),
        _sunDirection(glm::normalize(glm::dvec3(0.8017, 0.2673, 0.5345))),
        _spaceMaterial(material::AIR)
    {

    }

    void ProceduralSun::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    void ProceduralSun::setSunColor(const glm::dvec3& color)
    {
        _sunColor = color;

        stampCurrentUpdate();
    }

    void ProceduralSun::setSkyColor(const glm::dvec3& color)
    {
        _skyColor = color;

        stampCurrentUpdate();
    }

    void ProceduralSun::setSkylineColor(const glm::dvec3& color)
    {
        _skylineColor = color;

        stampCurrentUpdate();
    }

    void ProceduralSun::setGroundColor(const glm::dvec3& color)
    {
        _groundColor = color;

        stampCurrentUpdate();
    }

    void ProceduralSun::setGroundHeight(double height)
    {
        _groundHeight = height;

        stampCurrentUpdate();
    }

    void ProceduralSun::setSunDirection(const glm::dvec3& dir)
    {
        _sunDirection = dir;

        stampCurrentUpdate();
    }

    glm::dvec4 ProceduralSun::raycast(const Raycast& ray) const
    {
        glm::dvec3 color;

        double dotNormTop = ray.direction.z;
        double dotSunTop = _sunDirection.z;
        double dotDirSun = glm::dot(ray.direction, _sunDirection);

        // Diffuse scattering
        {
            glm::dvec3 topColor = kelvinToRgb(100000);
            glm::dvec3 bottomColor = kelvinToRgb(glm::mix(1000, 6600, dotSunTop));

            double daylighIntens = 1.0 - glm::pow(1.0 - glm::max(0.0, (dotSunTop + 0.05) / 1.05), 5.0);

            color += glm::mix(bottomColor, topColor, (1 + dotNormTop) / 2.0)
                        * daylighIntens;
        }

        // Halo scattering
        {
            double sunMinHeight = -0.1;
            double sunHeightRatio = glm::max(0.0, (dotSunTop - sunMinHeight) / (1.0 - sunMinHeight));
            double dirSunRatio = (dotDirSun + 1.0) / 2.0;

            double haloTemp = glm::pow(sunHeightRatio, 32.0);
            glm::dvec3 haloColor = kelvinToRgb(glm::mix(1000, 6600, haloTemp));

            double haloIntens = glm::pow(dirSunRatio, 16.0);

            color += haloColor * haloIntens * 0.5;
        }


        // Final blended color
        double distance = ray.distance + BACKDROP_DISTANCE;
        double skyColorWeight =  1.0 / (distance *distance);
        return glm::dvec4(color * skyColorWeight, skyColorWeight);
    }

    std::vector<Raycast> ProceduralSun::fireRays(unsigned int count) const
    {
        // Fire only sun rays
        double sunRayProb = (1.0 - (SUN_COS_DIMENSION + 1.0)/2.0);
        glm::dvec3 rayColor = _sunColor * (sunRayProb / count) * 32.0;
        glm::dvec4 sunSample(rayColor * sunRayProb, sunRayProb);

        glm::dvec3 sideward = glm::normalize(glm::cross(SKY_UP, _sunDirection));
        glm::dvec3 upward = glm::normalize(glm::cross(sideward, _sunDirection));
        std::vector<Raycast> raycasts;
        for(unsigned int i=0; i < count; ++i)
        {
            glm::dvec2 recDist = glm::diskRand(RECEIVE_PLANE_RADIUS);
            glm::dvec3 recPoint = RECEIVE_PLANE_POS +
                sideward * recDist.x + upward * recDist.y;

            glm::dvec2 radDist = glm::diskRand(RADIATION_PLANE_RADIUS);
            glm::dvec3 radPoint = recPoint +
                _sunDirection * RADIATION_PLANE_DISTANCE +
                sideward * radDist.x + upward * radDist.y;

            glm::dvec3 direction = glm::normalize(recPoint - radPoint);
            raycasts.push_back(Raycast(
                Raycast::BACKDROP_LIMIT,
                Raycast::INITIAL_DISTANCE,
                sunSample,
                radPoint,
                direction));
        }

        return raycasts;
    }

    std::vector<Raycast> ProceduralSun::fireOn(
                    const glm::dvec3& pos,
                    unsigned int count) const
    {
        // Fire only sun rays
        double sunRayProb = (1.0 - (SUN_COS_DIMENSION + 1.0)/2.0);
        glm::dvec3 rayColor = _sunColor * (sunRayProb / count);
        glm::dvec4 sunSample(rayColor * sunRayProb, sunRayProb);

        glm::dvec3 sideward = glm::normalize(glm::cross(SKY_UP, _sunDirection));
        glm::dvec3 upward = glm::normalize(glm::cross(sideward, _sunDirection));
        std::vector<Raycast> raycasts;
        for(unsigned int i=0; i < count; ++i)
        {
            glm::dvec2 radDist = glm::diskRand(RADIATION_PLANE_RADIUS);
            glm::dvec3 radPoint =
                pos + _sunDirection * RADIATION_PLANE_DISTANCE +
                sideward * radDist.x +
                upward * radDist.y;

            glm::dvec3 direction = glm::normalize(pos - radPoint);
            raycasts.push_back(Raycast(
                Raycast::BACKDROP_LIMIT,
                Raycast::INITIAL_DISTANCE,
                sunSample,
                radPoint,
                direction));
        }

        return raycasts;
    }
}
