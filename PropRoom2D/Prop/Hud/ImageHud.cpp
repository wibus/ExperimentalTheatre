#include "ImageHud.h"


namespace prop2
{
    ImageHud::ImageHud() :
        AbstractHud(EPropType::IMAGE_HUD),        
        _imageName(""),
        _cornersPositions(4),
        _cornersTexCoords(4),
        _colorFilter(1.0, 1.0, 1.0, 1.0)
    {
        _cornersPositions[0] = glm::dvec2(0.0, 0.0);
        _cornersPositions[1] = glm::dvec2(1.0, 0.0);
        _cornersPositions[2] = glm::dvec2(1.0, 1.0);
        _cornersPositions[3] = glm::dvec2(0.0, 1.0);

        _cornersTexCoords[0] = glm::dvec2(0.0, 0.0);
        _cornersTexCoords[1] = glm::dvec2(1.0, 0.0);
        _cornersTexCoords[2] = glm::dvec2(1.0, 1.0);
        _cornersTexCoords[3] = glm::dvec2(0.0, 1.0);
    }

    ImageHud::~ImageHud()
    {
    }

    std::string ImageHud::imageName() const
    {
        return _imageName;
    }

    glm::dvec2 ImageHud::handlePosition() const
    {
        return _cornersPositions[0];
    }

    glm::dvec2 ImageHud::size() const
    {
        return glm::dvec2(width(), height());
    }

    double ImageHud::width() const
    {
        return _cornersPositions[1].x - _cornersPositions[0].x;
    }

    double ImageHud::height() const
    {
        return _cornersPositions[3].y - _cornersPositions[0].y;
    }

    glm::dvec2 ImageHud::texOrigin() const
    {
        return _cornersTexCoords[0];
    }

    glm::dvec2 ImageHud::texExtents() const
    {
        return glm::dvec2(_cornersTexCoords[1].x - _cornersTexCoords[0].x,
                          _cornersTexCoords[2].y - _cornersTexCoords[1].y);
    }

    void ImageHud::setImageName(const std::string name)
    {
        _imageName = name;
    }

    void ImageHud::setHandlePosition(const glm::dvec2& position)
    {
        glm::dvec2 displacement = position - handlePosition();
        _cornersPositions[0] += displacement;
        _cornersPositions[1] += displacement;
        _cornersPositions[2] += displacement;
        _cornersPositions[3] += displacement;
    }

    void ImageHud::setSize(const glm::dvec2& size)
    {
        setWidth(size.x);
        setHeight(size.y);
    }

    void ImageHud::setWidth(double width)
    {
        _cornersPositions[1].x =_cornersPositions[0].x + width;
        _cornersPositions[2].x =_cornersPositions[0].x + width;
    }

    void ImageHud::setHeight(double height)
    {
        _cornersPositions[2].y =_cornersPositions[0].y + height;
        _cornersPositions[3].y =_cornersPositions[0].y + height;
    }

    void ImageHud::setTexOrigin(const glm::dvec2& origin)
    {
        glm::dvec2 extents = texExtents();
        _cornersTexCoords[0] = glm::dvec2(origin.x,             origin.y);
        _cornersTexCoords[1] = glm::dvec2(origin.x + extents.x, origin.y);
        _cornersTexCoords[2] = glm::dvec2(origin.x + extents.x, origin.y + extents.y);
        _cornersTexCoords[3] = glm::dvec2(origin.x,             origin.y + extents.y);
    }

    void ImageHud::setTexExtents(const glm::dvec2& extents)
    {
        glm::dvec2 origin = texOrigin();
        _cornersTexCoords[1].x = origin.x + extents.x;
        _cornersTexCoords[2] = glm::dvec2(origin.x + extents.x, origin.y + extents.y);
        _cornersTexCoords[3].y = origin.y + extents.y;
    }
}
