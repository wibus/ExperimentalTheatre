#include "EmissiveCoating.h"

#include "Node/Visitor.h"
#include "Ray/RayHitReport.h"


namespace prop3
{
    EmissiveCoating::EmissiveCoating(
            const glm::dvec3& emittedRadiance) :
        _emittedRadiance()
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
        return glm::dvec4(_emittedRadiance, 1.0);
    }

    glm::dvec4 EmissiveCoating::directBrdf(
            const RayHitReport& report,
            const Raycast& incidentRay,
            const glm::dvec3& outDirection) const
    {
        return glm::dvec4(0.0, 0.0, 0.0, 1.0);
    }

    glm::dvec3 EmissiveCoating::albedo(
            const RayHitReport& report) const
    {
        return _emittedRadiance;
    }

    void EmissiveCoating::setEmittedRadiance(
            const glm::dvec3& radiance)
    {
        _emittedRadiance = radiance;

        stampCurrentUpdate();
    }
}
