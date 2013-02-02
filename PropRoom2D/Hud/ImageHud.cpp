#include "ImageHud.h"


namespace prop2
{
    ImageHud::ImageHud() :
        AbstractHud(PropType::IMAGE_HUD),        
        _imageName(""),
        _cornersPositions(4),
        _cornersTexCoords(4),
        _colorFilter(real(1.0), real(1.0), real(1.0), real(1.0))
    {
        _cornersPositions[0](real(0.0), real(0.0));
        _cornersPositions[1](real(1.0), real(0.0));
        _cornersPositions[2](real(1.0), real(1.0));
        _cornersPositions[3](real(0.0), real(1.0));

        _cornersTexCoords[0](real(0.0), real(0.0));
        _cornersTexCoords[1](real(1.0), real(0.0));
        _cornersTexCoords[2](real(1.0), real(1.0));
        _cornersTexCoords[3](real(0.0), real(1.0));
    }

    ImageHud::~ImageHud()
    {
    }

    std::string ImageHud::imageName() const
    {
        return _imageName;
    }

    Vec2r ImageHud::handlePosition() const
    {
        return _cornersPositions[0];
    }

    Vec2r ImageHud::size() const
    {
        return Vec2r(width(), height());
    }

    real ImageHud::width() const
    {
        return _cornersPositions[1].x() - _cornersPositions[0].x();
    }

    real ImageHud::height() const
    {
        return _cornersPositions[3].y() - _cornersPositions[0].y();
    }

    Vec2r ImageHud::texOrigin() const
    {
        return _cornersTexCoords[0];
    }

    Vec2r ImageHud::texExtents() const
    {
        return Vec2r(_cornersTexCoords[1].x() - _cornersTexCoords[0].x(),
                     _cornersTexCoords[2].y() - _cornersTexCoords[1].y());
    }

    void ImageHud::setImageName(const std::string name)
    {
        _imageName = name;
    }

    void ImageHud::setHandlePosition(const Vec2r& position)
    {
        Vec2r displacement = position - handlePosition();
        _cornersPositions[0] += displacement;
        _cornersPositions[1] += displacement;
        _cornersPositions[2] += displacement;
        _cornersPositions[3] += displacement;
    }

    void ImageHud::setSize(const Vec2r& size)
    {
        setWidth(size.x());
        setHeight(size.y());
    }

    void ImageHud::setWidth(real width)
    {
        _cornersPositions[1].setX(_cornersPositions[0].x() + width);
        _cornersPositions[2].setX(_cornersPositions[0].x() + width);
    }

    void ImageHud::setHeight(real height)
    {
        _cornersPositions[2].setY(_cornersPositions[0].y() + height);
        _cornersPositions[3].setY(_cornersPositions[0].y() + height);
    }

    void ImageHud::setTexOrigin(const Vec2r& origin)
    {
        Vec2r extents = texExtents();
        _cornersTexCoords[0](origin.x(),               origin.y());
        _cornersTexCoords[1](origin.x() + extents.x(), origin.y());
        _cornersTexCoords[2](origin.x() + extents.x(), origin.y() + extents.y());
        _cornersTexCoords[3](origin.x(),               origin.y() + extents.y());
    }

    void ImageHud::setTexExtents(const Vec2r& extents)
    {
        Vec2r origin = texOrigin();
        _cornersTexCoords[1].setX(origin.x() + extents.x());
        _cornersTexCoords[2](origin.x() + extents.x(), origin.y() + extents.y());
        _cornersTexCoords[3].setY( origin.y() + extents.y());
    }
}
