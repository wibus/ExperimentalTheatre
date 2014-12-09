#ifndef PROPROOM3D_SPACE_EQUATION_H
#define PROPROOM3D_SPACE_EQUATION_H

#include <memory>
#include <vector>

#include <GLM/glm.hpp>

#include "libPropRoom3D_global.h"


namespace prop3
{
class Ray;
class RaycastReport;

    enum class EPointPosition
    {
        OUT,
        ON,
        IN
    };

    class PROP3D_EXPORT SpaceEquation
    {
    protected:
        SpaceEquation();

    public:
        virtual ~SpaceEquation();

        EPointPosition isIn(double x, double y, double z) const;
        virtual EPointPosition isIn(const glm::dvec3& point) const = 0;

        double signedDistance(double x, double y, double z) const;
        virtual double signedDistance(const glm::dvec3& point) const = 0;

        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const =0;
    };



    class PROP3D_EXPORT EqNot : public SpaceEquation
    {
    public:
        EqNot(const std::shared_ptr<SpaceEquation>& eq);

        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;

    private:
        std::shared_ptr<SpaceEquation> _eq;
    };



    class PROP3D_EXPORT EqOr : public SpaceEquation
    {
    public:
        EqOr(const std::shared_ptr<SpaceEquation>& eq1,
             const std::shared_ptr<SpaceEquation>& eq2);
        EqOr(const std::vector<std::shared_ptr<SpaceEquation>>& eqs);

        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;

    private:
        std::vector<std::shared_ptr<SpaceEquation>> _eqs;
    };



    class PROP3D_EXPORT EqAnd : public SpaceEquation
    {
    public:
        EqAnd(const std::shared_ptr<SpaceEquation>& eq1,
             const std::shared_ptr<SpaceEquation>& eq2);
        EqAnd(const std::vector<std::shared_ptr<SpaceEquation>>& eqs);

        virtual EPointPosition isIn(const glm::dvec3& point) const;
        virtual double signedDistance(const glm::dvec3& point) const;
        virtual void raycast(const Ray& ray, std::vector<RaycastReport>& reports) const;

    private:
        std::vector<std::shared_ptr<SpaceEquation>> _eqs;
    };


    // Logical operators overloading
    inline std::shared_ptr<SpaceEquation> operator!(
            const std::shared_ptr<SpaceEquation>& eq)
    {
        return std::shared_ptr<SpaceEquation>(new EqNot(eq));
    }

    inline std::shared_ptr<SpaceEquation> operator| (
            const std::shared_ptr<SpaceEquation>& eq1,
            const std::shared_ptr<SpaceEquation>& eq2)
    {
        return std::shared_ptr<SpaceEquation>(new EqOr(eq1, eq2));
    }

    inline std::shared_ptr<SpaceEquation> operator& (
            const std::shared_ptr<SpaceEquation>& eq1,
            const std::shared_ptr<SpaceEquation>& eq2)
    {
        return std::shared_ptr<SpaceEquation>(new EqAnd(eq1, eq2));
    }



    // IMPLEMENTATION //
    inline EPointPosition SpaceEquation::isIn(double x, double y, double z) const
    {
        return isIn(glm::dvec3(x, y, z));
    }

    inline double SpaceEquation::signedDistance(double x, double y, double z) const
    {
        return signedDistance(glm::dvec3(x, y, z));
    }
}

#endif // PROPROOM3D_SPACE_EQUATION_H
