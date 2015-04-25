#include "TexturedPaint.h"

#include <GLM/gtc/random.hpp>

#include <MediaWorkbench/Image/Image.h>
#include <MediaWorkbench/Image/ImageBank.h>

namespace prop3
{
    TexturedPaint::TexturedPaint(const std::string& texName,
                                 const glm::dvec3& defaultColor) :
        FlatPaint(defaultColor),
        _texture(media::getImageBank().getImage(texName))
    {
    }

    TexturedPaint::~TexturedPaint()
    {

    }

    std::shared_ptr<Costume> TexturedPaint::clone() const
    {
        return std::shared_ptr<Costume>(new TexturedPaint(*this));
    }

    glm::dvec3 TexturedPaint::computeReflectionBrdf(
            const glm::dvec3& incidentDirection,
            const glm::dvec3& surfaceNormal,
            const glm::dvec3& viewDirection,
            const glm::dvec3& texCoord) const
    {
        if(texCoord == glm::dvec3())
            return mediumColor();

        unsigned char* pixel = _texture.pixel(
                glm::clamp((int)(texCoord.s * _texture.width()), 0, _texture.width()-1),
                glm::clamp((int)(texCoord.t * _texture.height()), 0, _texture.height()-1));

        glm::dvec3 color(
                pixel[0] / 255.0,
                pixel[1] / 255.0,
                pixel[2] / 255.0);

        return color * glm::max(0.0,
            -glm::dot(incidentDirection, surfaceNormal));
    }
}
