#ifndef PROPROOM3D_FLATPAINT_H
#define PROPROOM3D_FLATPAINT_H

#include "Costume.h"


namespace prop3
{

    class PROP3D_EXPORT FlatPaint :
            public Costume
    {
    public:
        FlatPaint(const glm::dvec3& color);
        virtual ~FlatPaint();

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

#endif // PROPROOM3D_FLATPAINT_H
