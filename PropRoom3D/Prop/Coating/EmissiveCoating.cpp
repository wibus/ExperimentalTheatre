#include "EmissiveCoating.h"

#include "Node/Visitor.h"
#include "Ray/RayHitReport.h"
#include "Light/LightBulb/LightBulb.h"


namespace prop3
{
    EmissiveCoating::EmissiveCoating(const LightBulb& lightBulb) :
        _lightBulb(lightBulb)
    {

    }

    EmissiveCoating::~EmissiveCoating()
    {

    }

    void EmissiveCoating::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    glm::dvec4 EmissiveCoating::indirectBrdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const Raycast& incidentRay) const
    {
        glm::dvec3 virtOrig = report.position - incidentRay.direction * incidentRay.virtDist;
        double visibility = _lightBulb.visibility(Raycast(incidentRay.entropy, incidentRay.sample, virtOrig, incidentRay.direction));
        return glm::dvec4(_lightBulb.radiantFlux() / _lightBulb.area() * visibility, visibility);
    }

    glm::dvec4 EmissiveCoating::directBrdf(
            const LightCast& lightCast,
            const RayHitReport& report,
            const Raycast& eyeRay) const
    {
        return glm::dvec4(0.0, 0.0, 0.0, 0.0);
    }

    glm::dvec3 EmissiveCoating::albedo(
            const RayHitReport& report) const
    {
        if(_lightBulb.isOn())
            return _lightBulb.radiantFlux() / _lightBulb.area();
        else
            glm::dvec3(0.0, 0.0, 0.0);
    }
}
