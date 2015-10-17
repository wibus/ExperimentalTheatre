#include "Glass.h"

#include "Ray/Raycast.h"
#include "Ray/RayUtils.h"
#include "../../StageSet/StageSetVisitor.h"


namespace prop3
{
    Glass::Glass(const glm::dvec3& color,
                 double concentration,
                 double refractiveIndex) :
        _color(color),
        _concentration(concentration)
    {
        setRefractiveIndex(refractiveIndex);
    }

    Glass::~Glass()
    {

    }

    void Glass::indirectBrdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const Material& leavedMaterial,
        const Material& enteredMaterial,
        unsigned int outRayCountHint) const
    {
        indirectSpecularRefraction(
            raycasts,
            report,
            leavedMaterial.refractiveIndex(),
            enteredMaterial.refractiveIndex());
    }

    glm::dvec3 Glass::directBrdf(
        const RayHitReport& report,
        const glm::dvec3& outDirection,
        const Material& leavedMaterial,
        const Material& enteredMaterial) const
    {
        return directSpecularRefraction(report, outDirection);
    }

    glm::dvec3 Glass::lightAttenuation(const Raycast& ray) const
    {
        if(_concentration == 0.0)
            return glm::dvec3(1.0);

        return glm::pow(_color, glm::dvec3(_concentration * ray.limit));
    }

    void Glass::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }
}
