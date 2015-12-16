#include "ProceduralSun.h"

#include <GLM/gtc/random.hpp>
#include <GLM/gtc/constants.hpp>

#include "Light/LightUtils.h"
#include "Prop/Material/Material.h"
#include "Ray/Raycast.h"
#include "Ray/RayHitList.h"
#include "Node/Visitor.h"


namespace prop3
{
    const double ProceduralSun::SUN_COS_RADIUS = 0.9999890722;
    const double ProceduralSun::SUN_SURFACE_RATIO = 0.00001092779;
    const glm::dvec3 ProceduralSun::SKY_UP = glm::dvec3(0, 0, 1);

    const double ProceduralSun::RECEIVE_PLANE_RADIUS = 30.0;
    const glm::dvec3 ProceduralSun::RECEIVE_PLANE_POS =
        glm::dvec3(0.0, 0.0, 0.0);

    const double ProceduralSun::RADIATION_PLANE_DISTANCE = 30.0;
    const double ProceduralSun::RADIATION_PLANE_RADIUS =
            ProceduralSun::RADIATION_PLANE_DISTANCE *
            0.00467503405;


    const double MIN_DIR_HEIGHT = -0.25;
    const double MIN_SUN_HEIGHT = -0.00;
    const double DIFFUSE_POWER = 2.0;
    const double HALO_POWER = 32.0;
    const double DAY_POWER = 1.0;

    ProceduralSun::ProceduralSun() :
        _sunColor(glm::dvec3(1.0/*1.00, 0.75, 0.62*/) * 6e4),
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
        double dotDirSun = glm::dot(ray.direction, _sunDirection);
        double dotDirTop = ray.direction.z;
        double dotSunTop = _sunDirection.z;


        double dayHeight (dotSunTop - MIN_SUN_HEIGHT);
        double dayRatio = glm::max(0.0, dayHeight / (1.0 - MIN_SUN_HEIGHT));
        double dayMix = 1.0 - glm::pow(1.0 - dayRatio, DAY_POWER);

        glm::dvec3 topColor = kelvinToRgb(100000);
        glm::dvec3 bottomColor = kelvinToRgb(glm::mix(2000, 6600, dayMix));
        glm::dvec3 haloColor = kelvinToRgb(glm::mix(2000, 6600, dayMix));

        double diffuseHeight (dotDirTop - MIN_DIR_HEIGHT);
        double diffuseRatio = glm::max(0.0, diffuseHeight / (1.0 - MIN_DIR_HEIGHT));
        double diffuseMix = 1.0 - glm::pow(1.0 - diffuseRatio, DIFFUSE_POWER);
        glm::dvec3 diffuseColor = glm::mix(bottomColor, topColor, diffuseMix);
        double diffuseIntens = glm::mix(0.05, 1.0, 1.0 - glm::pow(1.0 - dayRatio, 4.0));

        double haloRatio = (1.0 + dotDirSun) / 2.0;
        double haloMix = glm::pow(haloRatio, HALO_POWER);
        double halowIntensity = glm::mix(0.25, 1.0, 1.0 - glm::pow(1.0 - dayRatio, 4.0));

        glm::dvec3 skyColor = glm::mix(
            diffuseColor *diffuseIntens,
            haloColor * halowIntensity,
            haloMix);

        return glm::dvec4(skyColor, 1.0);
    }

    std::vector<Raycast> ProceduralSun::fireRays(unsigned int count) const
    {
        // Fire only sun rays
        glm::dvec3 rayColor = _sunColor / double(count);
        glm::dvec4 sunSample(rayColor * SUN_SURFACE_RATIO, SUN_SURFACE_RATIO);

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
        double dotSunTop = _sunDirection.z;
        double dayHeight (dotSunTop - MIN_SUN_HEIGHT);
        double dayRatio = glm::max(0.0, dayHeight / (1.0 - MIN_SUN_HEIGHT));
        double dayMix = 1.0 - glm::pow(1.0 - dayRatio, DAY_POWER);
        glm::dvec3 haloFilter = kelvinToRgb(glm::mix(2000, 6600, dayMix));

        glm::dvec3 rayColor = haloFilter * _sunColor / double(count);
        glm::dvec4 sunSample(rayColor * SUN_SURFACE_RATIO, SUN_SURFACE_RATIO);

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
