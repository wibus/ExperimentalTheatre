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
    const double DIFFUSE_POWER = 2.0;
    const double HALO_POWER = 32.0;
    const double DAY_POWER = 1.0;

    double sunDiffuseSize(
            const LightCast& lightCast,
            const Raycast& eyeRay,
            double roughness)
    {
        double entropy = Raycast::totalEntropy(eyeRay, lightCast.raycast, roughness);
        return cellar::fast_pow(ProceduralSun::SUN_SURFACE_RATIO, entropy);
    }

    ProceduralSun::ProceduralSun() :
        _sunColor(glm::dvec3(1.0/*1.00, 0.75, 0.62*/) * 1.75e5),
        _skyColor(glm::dvec3(0.35, 0.40, 1.00) * 1.0),
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

    double ProceduralSun::distance(const Raycast& ray) const
    {
        return 20.0;
    }

    glm::dvec4 ProceduralSun::raycast(const Raycast& ray) const
    {
        double dotDirSun = glm::dot(ray.direction, _sunDirection);
        double dotDirTop = ray.direction.z;
        double dotSunTop = _sunDirection.z;


        double dayHeight(dotSunTop - MIN_SUN_HEIGHT);
        double dayRatio = glm::max(0.0, dayHeight / (1.0 - MIN_SUN_HEIGHT));
        double dayMix = 1.0 - cellar::fast_pow(1.0 - dayRatio, DAY_POWER);

        glm::dvec3 diffuseColor;
        if(dotDirTop >= MIN_DIR_HEIGHT)
            diffuseColor = _skyColor;
        else
            diffuseColor = glm::dvec3(glm::mix(0.1, 0.5, dayMix));

        glm::dvec3 skylineColor = kelvinToRgb(glm::mix(3000, 8000, dayMix));
        glm::dvec3 haloColor = kelvinToRgb(glm::mix(3000, 6600, dayMix));

        double diffuseHeight = (dotDirTop - MIN_DIR_HEIGHT);
        double diffuseRatio = glm::abs(diffuseHeight / (1.0 - MIN_DIR_HEIGHT));
        double diffuseMix = 1.0 - cellar::fast_pow(1.0 - diffuseRatio, DIFFUSE_POWER);
        diffuseColor = glm::mix(skylineColor, diffuseColor, diffuseMix);

        double haloRatio = (1.0 + dotDirSun) / 2.0;
        double haloMix = cellar::fast_pow(haloRatio, HALO_POWER);
        double halowIntensity = glm::mix(0.25, 1.0, 1.0 - cellar::fast_pow(1.0 - dayRatio, 4.0));
        double diffuseIntens = glm::mix(0.05, 1.0, 1.0 - cellar::fast_pow(1.0 - dayRatio, 4.0));

        glm::dvec3 skyColor = glm::mix(
            diffuseColor * diffuseIntens,
            haloColor * halowIntensity,
            haloMix);


        glm::dvec4 sunSample;

        double ringWidth = (1.0 - SUN_COS_RADIUS);
        if(dotDirSun > SUN_COS_RADIUS - ringWidth)
        {
            glm::dvec3 sunTint = kelvinToRgb(glm::mix(2000, 6600, dayMix));
            glm::dvec3 sunColor = sunTint * _sunColor;
            double sunProb = cellar::fast_pow(SUN_SURFACE_RATIO, ray.entropy);
            if(dotDirSun > SUN_COS_RADIUS)
            {
                sunSample = glm::dvec4(sunColor * sunProb, sunProb);
            }
            else
            {
                double alpha = (dotDirSun - (SUN_COS_RADIUS - ringWidth)) / ringWidth;
                double attenuation = cellar::fast_pow(0.3, 1.0/(1.0/(1.0-alpha) - 1.0)*3.0);
                sunSample = glm::dvec4((skyColor + sunColor * attenuation) * sunProb, sunProb);
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

        double dayMix = 1.0 - cellar::fast_pow(1.0 - dayRatio, DAY_POWER);
        glm::dvec3 haloFilter = kelvinToRgb(glm::mix(2000, 6600, dayMix));

        glm::dvec4 sunSample(haloFilter * _sunColor, 1.0);

        glm::dvec3 sideward = glm::normalize(glm::cross(SKY_UP, _sunDirection));
        glm::dvec3 upward = glm::normalize(glm::cross(sideward, _sunDirection));
        for(unsigned int i=0; i < count; ++i)
        {
            glm::dvec2 radDist = _diskRand.gen(RADIATION_PLANE_RADIUS);
            glm::dvec3 radPoint =
                pos + _sunDirection * RADIATION_PLANE_DISTANCE +
                sideward * radDist.x +
                upward * radDist.y;

            glm::dvec3 direction = glm::normalize(pos - radPoint);
            Raycast raycasts(
                Raycast::FULLY_SPECULAR,
                sunSample,
                radPoint,
                direction);

            lightCasts.push_back(LightCast(raycasts, radPoint, direction, sunDiffuseSize));
        }
    }
}
