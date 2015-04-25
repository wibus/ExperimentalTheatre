#ifndef PROPROOM3D_GLASS_H
#define PROPROOM3D_GLASS_H

#include "Costume.h"


namespace prop3
{

    class PROP3D_EXPORT Glass :
            public Costume
    {
    public:
        Glass(const glm::dvec3& color = glm::dvec3(1.0));
        virtual ~Glass();

        virtual std::shared_ptr<Costume> clone() const;

        virtual double computeReflexionRatio(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& texCoord = glm::dvec3(0)) const;

        // Reflexion model
        virtual glm::dvec3 computeReflectionDirection(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& texCoord = glm::dvec3(0)) const;

        virtual glm::dvec3 computeReflectionBrdf(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& viewDirection,
                const glm::dvec3& texCoord = glm::dvec3(0)) const;

        virtual glm::dvec3 computeRefractionDirection(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& texCoord = glm::dvec3(0)) const;

        virtual glm::dvec3 computeRefractionBrdf(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& viewDirection,
                const glm::dvec3& texCoord = glm::dvec3(0)) const;

    private:
    };
}

#endif // PROPROOM3D_GLASS_H
