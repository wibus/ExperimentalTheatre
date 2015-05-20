#include "TexturedPaint.h"

#include <GLM/gtc/random.hpp>

#include <CellarWorkbench/Image/Image.h>
#include <CellarWorkbench/Image/ImageBank.h>


namespace prop3
{
    TexturedPaint::TexturedPaint(const std::string& texName,
                                 const glm::dvec3& defaultColor) :
        FlatPaint(defaultColor),
        _texture(cellar::getImageBank().getImage(texName))
    {
    }

    TexturedPaint::~TexturedPaint()
    {

    }

    glm::dvec3 TexturedPaint::computeReflectionBrdf(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& viewDirection,
            const glm::dvec3& texCoord) const
    {
        glm::dvec3 color = mediumColor();

        if(texCoord != glm::dvec3())
        {
            int i = texCoord.s * _texture.width();
            int j = texCoord.t * _texture.height();
            unsigned char* pixel = _texture.pixel(
                glm::clamp(i, 0,  _texture.width()-1),
                glm::clamp(j, 0, _texture.height()-1));

            // Not blended with default color
            color.x = pixel[0] / 255.0;
            color.y = pixel[1] / 255.0;
            color.z = pixel[2] / 255.0;
        }

        return color * glm::max(0.0,
            -glm::dot(incidentDirection, surfaceNormal));
    }
}
