#include "ProceduralSun.h"

#include <GLM/gtc/random.hpp>
#include <GLM/gtc/constants.hpp>

#include "Prop/Material/Air.h"
#include "Prop/Ray/Raycast.h"
#include "Prop/Ray/RayHitList.h"
#include "StageSet/StageSetVisitor.h"

namespace prop3
{
    const double ProceduralSun::SUN_COS_DIMENSION = 0.99996192306;
    const double ProceduralSun::SUN_SMOOTH_EDGE = 5.0e4;
    const glm::dvec3 ProceduralSun::SKY_UP = glm::dvec3(0, 0, 1);

    const double ProceduralSun::SUN_SIN =
        glm::sqrt(1.0 - ProceduralSun::SUN_COS_DIMENSION * ProceduralSun::SUN_COS_DIMENSION);
    const glm::dvec3 ProceduralSun::RECEIVE_PLANE_POS =
        glm::dvec3(0.0, 0.0, 0.0);
    const double ProceduralSun::RECEIVE_PLANE_RADIUS = 30.0;

    const double ProceduralSun::RADIATION_PLANE_DISTANCE = 300.0;
    const double ProceduralSun::RADIATION_PLANE_RADIUS =
            ProceduralSun::RADIATION_PLANE_DISTANCE *
            ProceduralSun::SUN_SIN;

    ProceduralSun::ProceduralSun(bool isDirectlyVisible) :
        Backdrop(isDirectlyVisible),
        _sunColor(glm::dvec3(1.00, 0.80, 0.68) * 1e5),
        _skyColor(glm::dvec3(0.25, 0.60, 1.00) * 2.0),
        _skylineColor(glm::dvec3(1.00, 1.00, 1.00) * 2.0),
        _groundColor(glm::dvec3(0.05, 0.05, 0.3)),
        _groundHeight(-0.2),
        _sunDirection(glm::normalize(glm::dvec3(0.8017, 0.2673, 0.5345))),
        _spaceMaterial(new Air())
    {

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

    glm::dvec3 ProceduralSun::raycast(const Raycast& ray, bool directView) const
    {
        glm::dvec3 color;

        // Sun ray compatibility with surface reflexion
        double sunCompatibility = 0.0;
        /*
            Raycast::compatibility(
               Raycast::FULLY_SPECULAR_ENTROPY,
               ray.entropy);
               */

        double upDot = glm::dot(SKY_UP, ray.direction);
        if(upDot >= _groundHeight)
        {
            if(sunCompatibility != 1.0)
            {
                // Skyline Weight
                double a = 1.0 - glm::abs(upDot);
                a *= a *= a *= a;

                if(upDot >= 0.0)
                {
                    // Sky Color
                    glm::dvec3 skyColor = _skyColor * glm::max(0.0, upDot);
                    color = glm::mix(skyColor, _skylineColor, a);
                }
                else
                {
                    // Ground Color
                    color = glm::mix(_groundColor, _skylineColor, a);
                }

                double lightDot = glm::dot(_sunDirection, ray.direction);
                lightDot = lightDot * glm::max(0.0, lightDot);
                lightDot *= lightDot;

                color += _sunColor * lightDot / 1.0e4;
            }

            if(sunCompatibility != 0.0)
            {
                // Sun is only visible when the sky is direclty observed
                double sunDot = glm::dot(_sunDirection, ray.direction);
                double sunProb = sunDot / SUN_COS_DIMENSION;
                double sunPow = glm::pow(glm::abs(sunProb), SUN_SMOOTH_EDGE) * glm::sign(sunProb);
                color += _sunColor * glm::max(0.0, sunPow) * sunCompatibility;
            }
        }
        else
        {
            color = (1.0 - sunCompatibility) * _groundColor;
        }


        // Final blended color
        return color;
    }

    std::vector<Raycast> ProceduralSun::fireRays(unsigned int count) const
    {
        // Fire only sun rays
        double sunRayProb = (1.0 - (SUN_COS_DIMENSION + 1.0)/2.0);
        glm::dvec3 rayColor = _sunColor * (sunRayProb / count) * 32.0;

        glm::dvec3 sideward = glm::normalize(glm::cross(SKY_UP, _sunDirection));
        glm::dvec3 upward = glm::normalize(glm::cross(sideward, _sunDirection));
        std::vector<Raycast> raycasts;
        for(int i=0; i < count; ++i)
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
                Raycast::BACKDROP_DISTANCE,
                Raycast::FULLY_DIFFUSIVE_ENTROPY,
                rayColor,
                radPoint,
                direction,
                _spaceMaterial));
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

        glm::dvec3 sideward = glm::normalize(glm::cross(SKY_UP, _sunDirection));
        glm::dvec3 upward = glm::normalize(glm::cross(sideward, _sunDirection));
        std::vector<Raycast> raycasts;
        for(int i=0; i < count; ++i)
        {
            glm::dvec2 radDist = glm::diskRand(RADIATION_PLANE_RADIUS);
            glm::dvec3 radPoint =
                pos + _sunDirection * RADIATION_PLANE_DISTANCE +
                sideward * radDist.x +
                upward * radDist.y;

            glm::dvec3 direction = glm::normalize(pos - radPoint);
            raycasts.push_back(Raycast(
                Raycast::BACKDROP_DISTANCE,
                Raycast::FULLY_DIFFUSIVE_ENTROPY,
                rayColor,
                radPoint,
                direction,
                _spaceMaterial));
        }

        return raycasts;
    }

    void ProceduralSun::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
