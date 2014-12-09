#include "Brep.h"

namespace prop3
{
    Brep::Brep() :
        AbstractShape(EPropType::BREP)
    {

    }

    Brep::~Brep()
    {

    }

    bool Brep::contains(const glm::dvec3& point) const
    {
        return _eq->isIn(point) == EPointPosition::IN;
    }

    glm::dvec3 Brep::nearestSurface(const glm::dvec3& point) const
    {
        assert(false /* Not Implemented */);
        return point;
    }

    void Brep::raycast(const Ray& ray, std::vector<RaycastReport>& reports) const
    {
        _eq->raycast(ray, reports);
    }
}
