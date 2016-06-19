#include "ProceduralSun.h"

#include <CellarWorkbench/Misc/FastMath.h>

#include "Node/Light/LightCast.h"
#include "Node/Light/LightUtils.h"
#include "Node/Prop/Material/Material.h"
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
    const double MIN_SUN_HEIGHT = -0.05;
    const double HALO_POWER = 32.0;

    double sunDiffuseSize(
            const LightCast& lightCast,
            const Raycast& eyeRay,
            double roughness)
    {
        double entropy = Raycast::totalEntropy(eyeRay, lightCast.raycast, roughness);
        return cellar::fast_pow(ProceduralSun::SUN_SURFACE_RATIO, entropy);
    }

    ProceduralSun::ProceduralSun() :
        _sunIntensity(1.75e5),
        _skyColor(glm::dvec3(0.35, 0.40, 1.00) * 1.0),
        _groundHeight(-0.2),
        _sunDirection(glm::normalize(glm::dvec3(0.8017, 0.2673, 0.5345))),
        _spaceMaterial(material::AIR)
    {

    }

    void ProceduralSun::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    void ProceduralSun::setSunIntensity(double intensity)
    {
        _sunIntensity = intensity;

        stampCurrentUpdate();
    }

    void ProceduralSun::setSkyColor(const glm::dvec3& color)
    {
        _skyColor = color;

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
        _sunSideward = glm::normalize(glm::cross(SKY_UP, _sunDirection));
        _sunUpward = glm::normalize(glm::cross(_sunSideward, _sunDirection));

        double dayHeight = _sunDirection.z - MIN_SUN_HEIGHT;
        double dayRatio = glm::max(0.0, dayHeight / (1.0 - MIN_SUN_HEIGHT));

        _sunColor = _sunIntensity * kelvinToRgb(glm::mix(2000, 6400, dayRatio));
        _skylineColor = kelvinToRgb(glm::mix(3000, 8000, dayRatio));
        _haloColor = kelvinToRgb(glm::mix(3000, 6600, dayRatio));
        _groundColor = glm::dvec3(glm::mix(0.01, 0.5, dayRatio));

        _halowIntensity = glm::mix(0.10, 1.0, 1.0 - cellar::fast_pow(1.0 - dayRatio, 4.0));
        _diffuseIntens = glm::mix(0.01, 1.0, 1.0 - cellar::fast_pow(1.0 - dayRatio, 4.0));

        stampCurrentUpdate();
    }

    double ProceduralSun::distance(const Raycast& ray) const
    {
        return 20.0;
    }

    glm::dvec4 ProceduralSun::raycast(const Raycast& ray) const
    {
        double dotDirSun = glm::dot(ray.direction, _sunDirection);
        double dotDirTop = ray.direction.z;

        glm::dvec3 diffuseColor;
        if(dotDirTop >= MIN_DIR_HEIGHT)
            diffuseColor = _skyColor;
        else
            diffuseColor = _groundColor;

        double diffuseHeight = (dotDirTop - MIN_DIR_HEIGHT);
        double diffuseRatio = glm::abs(diffuseHeight / (1.0 - MIN_DIR_HEIGHT));
        double diffuseMix = 1.0 - (1.0 - diffuseRatio) * (1.0 - diffuseRatio);
        diffuseColor = glm::mix(_skylineColor, diffuseColor, diffuseMix);

        double haloRatio = (1.0 + dotDirSun) / 2.0;
        double haloMix = cellar::fast_pow(haloRatio, HALO_POWER);

        glm::dvec3 skyColor = glm::mix(
            diffuseColor * _diffuseIntens,
            _haloColor * _halowIntensity,
            haloMix);


        glm::dvec4 sunSample;

        const double RING_WIDTH = (1.0 - SUN_COS_RADIUS);
        if(dotDirSun > SUN_COS_RADIUS - RING_WIDTH)
        {

            double sunProb = cellar::fast_pow(SUN_SURFACE_RATIO, ray.entropy);

            if(dotDirSun > SUN_COS_RADIUS)
            {
                sunSample = glm::dvec4(_sunColor * sunProb, sunProb);
            }
            else
            {
                double alpha = (dotDirSun - (SUN_COS_RADIUS - RING_WIDTH)) / RING_WIDTH;
                double attenuation = cellar::fast_pow(0.3, 1.0/(1.0/(1.0-alpha) - 1.0)*3.0);
                sunSample = glm::dvec4((skyColor + _sunColor * attenuation) * sunProb, sunProb);
            }
        }

        return glm::dvec4(skyColor, 1.0) + sunSample;
    }

    void ProceduralSun::fireOn(
            std::vector<LightCast>& lightCasts,
            const glm::dvec3& pos,
            unsigned int count) const
    {
        // Fire only sun rays
        double dotSunTop = _sunDirection.z;
        double dayHeight (dotSunTop - MIN_SUN_HEIGHT);
        double dayRatio = dayHeight / (1.0 - MIN_SUN_HEIGHT);
        if(dayRatio <= MIN_SUN_HEIGHT)
            return;

        glm::dvec4 sunSample(_sunColor, 1.0);

        glm::dvec2 radDist = _diskRand.gen(RADIATION_PLANE_RADIUS);
        glm::dvec3 radPoint =
            pos + _sunDirection * RADIATION_PLANE_DISTANCE +
            _sunSideward * radDist.x +
            _sunUpward * radDist.y;

        glm::dvec3 direction = glm::normalize(pos - radPoint);
        Raycast raycasts(
            Raycast::FULLY_SPECULAR,
            sunSample,
            radPoint,
            direction);

        lightCasts.push_back(LightCast(raycasts, radPoint, direction, sunDiffuseSize));
    }
}
