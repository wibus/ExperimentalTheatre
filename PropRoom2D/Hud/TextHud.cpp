#include "TextHud.h"


namespace prop2
{
    TextHud::TextHud() :
        AbstractHud(EPropType::TEXT_HUD),
        _height(12.0),
        _handlePosition(0.0, 0.0),
        _text(""),
        _fontName("Arial"),
        _color(1.0, 1.0, 1.0, 1.0)
    {
    }

    TextHud::~TextHud()
    {
    }

    double TextHud::width() const
    {
        // TODO: wibus 2012-10-06: Forward text width computation to
        // implementation object dedicated to that computation.
        return 0;
    }

    double TextHud::height() const
    {
        return _height;
    }

    glm::dvec2 TextHud::handlePosition() const
    {
        return _handlePosition;
    }

    void TextHud::setHeight(double height)
    {
        _height = height;
    }

    void TextHud::setHandlePosition(const glm::dvec2& position)
    {
        _handlePosition = position;
    }
}
