#ifndef PROPROOM3D_CHROME_H
#define PROPROOM3D_CHROME_H

#include "Costume.h"


namespace prop3
{

    class PROP3D_EXPORT Chrome :
            public Costume
    {
    public:
        Chrome(const glm::dvec3& color = glm::dvec3(1.0));
        virtual ~Chrome();

        // Reflexion model
        virtual glm::dvec3 computeReflection(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& texCoord = glm::dvec3(0)) const;

        virtual glm::dvec3 computeReflectionBrdf(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& viewDirection,
                const glm::dvec3& texCoord = glm::dvec3(0)) const;

    private:
    };
}

#endif // PROPROOM3D_CHROME_H
