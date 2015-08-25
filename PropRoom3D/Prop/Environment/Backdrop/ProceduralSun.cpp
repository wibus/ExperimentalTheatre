#include "ProceduralSun.h"

#include "Prop/Ray/RayHitList.h"
#include "StageSet/StageSetVisitor.h"

namespace prop3
{
    ProceduralSun::ProceduralSun(bool isDirectlyVisible) :
        Backdrop(isDirectlyVisible),
        _sunColor(glm::dvec3(1.00, 0.7725, 0.5608) * 20.0),
        _skyColor(glm::dvec3(0.25, 0.60, 1.00) * 2.0),
        _skylineColor(glm::dvec3(1.00, 1.00, 1.00) * 2.0),
        _upSkyDirection(0, 0, 1),
        _sunDirection(0.8017, 0.2673, 0.5345)
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

    void ProceduralSun::setUpSkyDirection(const glm::dvec3& dir)
    {
        _upSkyDirection = dir;

        stampCurrentUpdate();
    }

    void ProceduralSun::setSunDirection(const glm::dvec3& dir)
    {
        _sunDirection = dir;

        stampCurrentUpdate();
    }

    glm::dvec3 ProceduralSun::raycast(const Ray& ray) const
    {
        // Sun Color
        double s = glm::max(0.0, glm::dot(_sunDirection, ray.direction));
        s *= s *= s *= s *= s;
        glm::dvec3 sun = _sunColor * s;

        // Sky Color
        double upDot = glm::dot(_upSkyDirection, ray.direction);
        glm::dvec3 sky = _skyColor * glm::max(0.0, upDot);

        // Skyline Color
        double a = 1.0 - glm::abs(upDot);
        a *= a *= a *= a;
        glm::dvec3 skyline = _skylineColor * a;


        // Final blended color
        return (sky + skyline + sun);
    }

    void ProceduralSun::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
