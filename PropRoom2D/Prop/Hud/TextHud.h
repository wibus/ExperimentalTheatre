#ifndef PROPROOM2D_TEXTHUD_H
#define PROPROOM2D_TEXTHUD_H

#include <string>

#include "AbstractHud.h"


namespace prop2
{
    class PROP2D_EXPORT TextHud : public AbstractHud
    {
    public:
        TextHud();
        virtual ~TextHud();

        // Getters
        virtual double width() const;
        virtual double height() const;
        virtual glm::dvec2 handlePosition() const;
        std::string text() const;
        std::string fontName() const;
        glm::dvec4 color() const;

        // Setters
        virtual void setHeight(double height);
        virtual void setHandlePosition(const glm::dvec2& position);
        void setText(const std::string text);
        void setFontName(const std::string name);
        void setColor(const glm::dvec4& color);

    private:
        double _height;
        glm::dvec2 _handlePosition;
        std::string _text;
        std::string _fontName;
        glm::dvec4 _color;
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

    inline glm::dvec4 TextHud::color() const
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

    inline void TextHud::setColor(const glm::dvec4& color)
    {
        _color = color;
    }
}

#endif // PROPROOM2D_TEXTHUD_H
