#include "EmissiveCoating.h"

#include "Serial/Visitor.h"
#include "Light/Sampler/Sampler.h"
#include "Ray/RayHitReport.h"


namespace prop3
{
    EmissiveCoating::EmissiveCoating()
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
        const Material& leavedMaterial,
        const Material& enteredMaterial) const
    {
        if(_sampler.get() != nullptr)
        {
            double probability = _sampler->hitProbability(report.incidentRay);
            return glm::dvec4(_emittedRadiance * probability, probability);
        }
        else
        {
            return glm::dvec4(0.0);
        }
    }

    glm::dvec4 EmissiveCoating::directBrdf(
        const RayHitReport& report,
        const glm::dvec3& outDirection,
        const Material& leavedMaterial,
        const Material& enteredMaterial) const
    {
        assert(false);
        return glm::dvec4(0);
    }

    glm::dvec3 EmissiveCoating::albedo(
            const RayHitReport& report) const
    {
        return _emittedRadiance;
    }

    void EmissiveCoating::setEmittedRadiance(const glm::dvec3& radiance)
    {
        _emittedRadiance = radiance;

        stampCurrentUpdate();
    }

    void EmissiveCoating::setSampler(const std::shared_ptr<Sampler>& sampler)
    {
        _sampler = sampler;

        stampCurrentUpdate();
    }
}
