#ifndef PROPROOM3D_BREP_H
#define PROPROOM3D_BREP_H

#include "AbstractShape.h"
#include "SpaceEquation/SpaceEquation.h"


namespace prop3
{
    class PROP3D_EXPORT Brep : public AbstractShape
    {
    public:
        Brep();
        virtual ~Brep();

        std::shared_ptr<SpaceEquation> equation() const;
        void setEquation(const std::shared_ptr<SpaceEquation>& eq);

        // Tests
        virtual bool contains(const glm::dvec3& point) const;
        virtual glm::dvec3 nearestSurface(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;

    private:
        std::shared_ptr<SpaceEquation> _eq;
    };



    // IMPLEMENTATION //
    inline std::shared_ptr<SpaceEquation> Brep::equation() const
    {
        return _eq;
    }

    inline void Brep::setEquation(const std::shared_ptr<SpaceEquation>& eq)
    {
        _eq = eq;
    }
}

#endif // PROPROOM3D_BREP_H
