#ifndef PROPROOM2D_TEXTHUD_H
#define PROPROOM2D_TEXTHUD_H

#include "libPropRoom2D_global.h"
#include "AbstractHud.h"

#include <string>


namespace prop2
{
    class PROP2D_EXPORT TextHud : public AbstractHud
    {
    public:
        TextHud();
        virtual ~TextHud();

        // Getters
        virtual real width() const;
        virtual real height() const;
        virtual Vec2r handlePosition() const;
        std::string text() const;
        std::string fontName() const;
        Vec4r color() const;

        // Setters
        virtual void setHeight(real height);
        virtual void setHandlePosition(const Vec2r& position);
        void setText(const std::string text);
        void setFontName(const std::string name);
        void setColor(const Vec4r& color);

    private:
        real _height;
        Vec2r _handlePosition;
        std::string _text;
        std::string _fontName;
        Vec4r _color;
    };



    // IMPLMENTATION //
    inline std::string TextHud::text() const
    {
        return _text;
    }

    inline std::string TextHud::fontName() const
    {
        return _fontName;
    }

    inline Vec4r TextHud::color() const
    {
        return _color;
    }

    inline void TextHud::setText(const std::string text)
    {
        _text = text;
    }

    inline void TextHud::setFontName(const std::string name)
    {
        _fontName = name;
    }

    inline void TextHud::setColor(const Vec4r& color)
    {
        _color = color;
    }
}

#endif // PROPROOM2D_TEXTHUD_H
