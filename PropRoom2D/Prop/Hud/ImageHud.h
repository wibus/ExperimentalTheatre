#ifndef PROPROOM2D_IMAGEHUD_H
#define PROPROOM2D_IMAGEHUD_H

#include <string>
#include <vector>

#include "AbstractHud.h"


namespace prop2
{
    class PROP2D_EXPORT ImageHud : public AbstractHud
    {
    public:
        ImageHud();
        virtual ~ImageHud();

        // Getters        
        virtual std::string imageName() const;
        virtual glm::dvec2 handlePosition() const;
        virtual glm::dvec2 size() const;
        virtual double width() const;
        virtual double height() const;
        virtual glm::dvec2 texOrigin() const;
        virtual glm::dvec2 texExtents() const;
        glm::dvec4 colorFilter() const;
        const std::vector<glm::dvec2>& cornersPositions() const;
        const std::vector<glm::dvec2>& cornersTexCoords() const;

        // Setters        
        virtual void setImageName(const std::string name);
        virtual void setHandlePosition(const glm::dvec2& position);
        virtual void setSize(const glm::dvec2& size);
        virtual void setWidth(double width);
        virtual void setHeight(double height);
        virtual void setTexOrigin(const glm::dvec2& origin);
        virtual void setTexExtents(const glm::dvec2& extents);
        virtual void setColorFilter(const glm::dvec4& color);

    private:
        std::string _imageName;
        std::vector<glm::dvec2> _cornersPositions;
        std::vector<glm::dvec2> _cornersTexCoords;
        glm::dvec4 _colorFilter;
    };



    // IMPLEMENTATION //
    inline const std::vector<glm::dvec2>& ImageHud::cornersPositions() const
    {
        return _cornersPositions;
    }

    inline const std::vector<glm::dvec2>& ImageHud::cornersTexCoords() const
    {
        return _cornersTexCoords;
    }

    inline glm::dvec4 ImageHud::colorFilter() const
    {
        return _colorFilter;
    }

    inline void ImageHud::setColorFilter(const glm::dvec4 &color)
    {
        _colorFilter = color;
    }
}

#endif // PROPROOM2D_IMAGEHUD_H
