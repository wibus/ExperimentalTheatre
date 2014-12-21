#ifndef PROPROOM3D_ABSTRACTCOSTUME_H
#define PROPROOM3D_ABSTRACTCOSTUME_H

#include "libPropRoom3D_global.h"

#include <memory>

#include <GLM/glm.hpp>


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
        glm::dvec3 mediumColor() const;
        void setMediumColor(const glm::dvec3& mediumColor);

        virtual double computeReflexionRatio(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& texCoord = glm::dvec3(0)) const;

        // Reflexion model
        virtual glm::dvec3 computeReflection(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& texCoord = glm::dvec3(0)) const = 0;

        virtual glm::dvec3 computeReflectionBrdf(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& viewDirection,
                const glm::dvec3& texCoord = glm::dvec3(0)) const = 0;

        // Refraction model
        virtual glm::dvec3 computeRefraction(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& texCoord = glm::dvec3(0)) const;

        virtual glm::dvec3 computeRefractionBrdf(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& viewDirection,
                const glm::dvec3& texCoord = glm::dvec3(0)) const;

    private:
        double _mediumOpacity;
        double _refractiveIndex;
        glm::dvec3 _mediumColor;
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

    inline glm::dvec3 AbstractCostume::mediumColor() const
    {
        return _mediumColor;
    }

    inline void AbstractCostume::setMediumColor(const glm::dvec3& color)
    {
        _mediumColor = color;
    }

    inline double AbstractCostume::computeReflexionRatio(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& texCoord) const
    {
        return 1.0;
    }

    inline glm::dvec3 AbstractCostume::computeRefraction(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& texCoord) const
    {
        return glm::dvec3();
    }

    inline glm::dvec3 AbstractCostume::computeRefractionBrdf(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& viewDirection,
            const glm::dvec3& texCoord) const
    {
        return glm::dvec3();
    }
}

#endif // PROPROOM3D_ABSTRACTCOSTUME_H
