#ifndef PROPROOM3D_TEXTUREDPAINT_H
#define PROPROOM3D_TEXTUREDPAINT_H

#include "FlatPaint.h"


namespace cellar
{
    class Image;
}

namespace prop3
{

    class PROP3D_EXPORT TexturedPaint :
            public FlatPaint
    {
    public:
        TexturedPaint(const std::string& texName,
                      const glm::dvec3& defaultColor = glm::dvec3(1.0));
        virtual ~TexturedPaint();

        // Reflexion model
        virtual glm::dvec3 computeReflectionBrdf(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& viewDirection,
                const glm::dvec3& texCoord = glm::dvec3(0)) const;

    private:
        cellar::Image& _texture;
    };
}

#endif // PROPROOM3D_TEXTUREDPAINT_H
