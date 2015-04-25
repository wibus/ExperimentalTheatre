#ifndef PROPROOM3D_TEXTURED_PAINT_H
#define PROPROOM3D_TEXTURED_PAINT_H

#include "FlatPaint.h"


namespace media
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

        virtual std::shared_ptr<Costume> clone() const;

        // Reflexion model
        virtual glm::dvec3 computeReflectionBrdf(
                const glm::dvec3& incidentDirection,
                const glm::dvec3& surfaceNormal,
                const glm::dvec3& viewDirection,
                const glm::dvec3& texCoord = glm::dvec3(0)) const;

    private:
        media::Image& _texture;
    };
}

#endif // PROPROOM3D_TEXTURED_PAINT_H
