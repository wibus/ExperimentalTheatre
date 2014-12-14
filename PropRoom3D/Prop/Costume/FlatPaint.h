#ifndef PROPROOM3D_FLAT_PAINT_H
#define PROPROOM3D_FLAT_PAINT_H

#include "AbstractCostume.h"


namespace prop3
{

    class PROP3D_EXPORT FlatPaint :
            public AbstractCostume
    {
    public:
        FlatPaint(const glm::vec3& color);
        virtual ~FlatPaint();

        // Reflexion model
        virtual glm::dvec3 computeReflection(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::vec3& texCoord = glm::vec3(0)) const;

        virtual glm::dvec3 computeRefraction(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::vec3& texCoord = glm::vec3(0)) const;

        virtual double computeReflexionRatio(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::vec3& texCoord = glm::vec3(0)) const;

        virtual glm::vec3 computeBrdf(
                const glm::vec3& incomingRadiosity,
                const glm::dvec3& lightDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& viewDirection,
                const glm::vec3& texCoord = glm::vec3(0)) const;

    private:
    };
}

#endif // PROPROOM3D_FLAT_PAINT_H
