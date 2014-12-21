#include "Glass.h"


namespace prop3
{
    Glass::Glass(const glm::dvec3& color)
    {
        setRefractiveIndex(1.5);
        setMediumColor(color);
    }

    Glass::~Glass()
    {

    }

    double Glass::computeReflexionRatio(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& texCoord) const
    {
        // Schlick's approximation
        double R0 = (1.0 - refractiveIndex()) / (1.0 + refractiveIndex());
        R0 = R0 * R0;

        double cosNV = 1.0 - glm::abs(glm::dot(incidentDirection, surfaceNormal));
        double cosNV5 = cosNV * cosNV * cosNV * cosNV * cosNV;
        return R0 + (1.0 - R0) * cosNV5;
    }

    glm::dvec3 Glass::computeReflection(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& texCoord) const
    {
        return glm::reflect(incidentDirection, surfaceNormal);
    }

    glm::dvec3 Glass::computeReflectionBrdf(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& viewDirection,
            const glm::dvec3& texCoord) const
    {
        return mediumColor();
    }

     glm::dvec3 Glass::computeRefraction(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& texCoord) const
    {
         bool isEntering = glm::dot(incidentDirection, surfaceNormal) < 0.0;
         double refractRatio = isEntering ? 1.0 / refractiveIndex() : refractiveIndex();
         glm::dvec3 projection = surfaceNormal * glm::dot(surfaceNormal, incidentDirection);

         glm::dvec3 z = refractRatio * (incidentDirection - projection);
         double zLength = glm::length(z);

         if(zLength < 1.0)
         {
            return z - surfaceNormal * (glm::sqrt(1.0 - zLength) *
                                        (isEntering ? 1.0 : -1.0));
         }
         else if(zLength > 1.0)
         {
            return incidentDirection - projection * 2.0;
         }
         else
         {
             return z;
         }
    }

    glm::dvec3 Glass::computeRefractionBrdf(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& viewDirection,
            const glm::dvec3& texCoord) const
    {
        return mediumColor();
    }
}
