#ifndef PROPROOM3D_ABSTRACTCOSTUME_H
#define PROPROOM3D_ABSTRACTCOSTUME_H

#include "libPropRoom3D_global.h"

#include <memory>

#include <glm/glm.hpp>


namespace prop3
{
    class PROP3D_EXPORT AbstractCostume
    {
    protected:
        AbstractCostume() {}

    public:
        virtual ~AbstractCostume() {}

        // Medium opacity
        double mediumOpacity() const;
        void setMediumOpacity(double mediumOpacity);

        // Medium refractive index
        double refractiveIndex() const;
        void setRefractiveIndex(double refractiveIndex);

        // Medium color
        glm::vec3 mediumColor() const;
        void setMediumColor(const glm::vec3& mediumColor);

        // Reflexion model
        virtual glm::dvec3 computeReflection(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal) const = 0;

        virtual glm::dvec3 computeRefraction(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal) const = 0;

        virtual double computeReflexionRatio(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal) const = 0;

        virtual glm::vec3 computeBrdf(
                const glm::vec3& incomingRadiosity,
                const glm::dvec3& lightDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& viewDirection) const = 0;

    private:
        double _mediumOpacity;
        double _refractiveIndex;
        glm::vec3 _mediumColor;
    };



    // IMPLEMENTATION //
    inline double AbstractCostume::mediumOpacity() const
    {
        return _mediumOpacity;
    }

    inline void AbstractCostume::setMediumOpacity(double opacity)
    {
        _mediumOpacity = opacity;
    }

    inline double AbstractCostume::refractiveIndex() const
    {
        return _refractiveIndex;
    }

    inline void AbstractCostume::setRefractiveIndex(double refractiveIndex)
    {
        _refractiveIndex = refractiveIndex;
    }

    inline glm::vec3 AbstractCostume::mediumColor() const
    {
        return _mediumColor;
    }

    inline void AbstractCostume::setMediumColor(const glm::vec3& color)
    {
        _mediumColor = color;
    }
}

#endif // PROPROOM3D_ABSTRACTCOSTUME_H
