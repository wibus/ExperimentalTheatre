#include "TextHud.h"


namespace prop2
{
    TextHud::TextHud() :
        AbstractHud(PropType::TEXT_HUD),
        _height(real(12.0)),
        _handlePosition(real(0.0), real(0.0)),
        _text(""),
        _fontName("Arial"),
        _color(real(1.0), real(1.0), real(1.0), real(1.0))
    {
    }

    TextHud::~TextHud()
    {
    }

    real TextHud::width() const
    {
        // TODO: wibus 2012-10-06: Forward text width computation to
        // implementation object dedicated to that computation.
        return real(0.0);
    }

    real TextHud::height() const
    {
        return _height;
    }

    Vec2r TextHud::handlePosition() const
    {
        return _handlePosition;
    }

    void TextHud::setHeight(real height)
    {
        _height = height;
    }

    void TextHud::setHandlePosition(const Vec2r& position)
    {
        _handlePosition = position;
    }
}
