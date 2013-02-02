#ifndef PROPROOM2D_IMAGEHUD_H
#define PROPROOM2D_IMAGEHUD_H

#include "libPropRoom2D_global.h"
#include "AbstractHud.h"

#include <string>
#include <vector>


namespace prop2
{
    class PROP2D_EXPORT ImageHud : public AbstractHud
    {
    public:
        ImageHud();
        virtual ~ImageHud();

        // Getters        
        virtual std::string imageName() const;
        virtual Vec2r handlePosition() const;
        virtual Vec2r size() const;
        virtual real width() const;
        virtual real height() const;
        virtual Vec2r texOrigin() const;
        virtual Vec2r texExtents() const;
        Vec4r colorFilter() const;
        const std::vector<Vec2r>& cornersPositions() const;
        const std::vector<Vec2r>& cornersTexCoords() const;

        // Setters        
        virtual void setImageName(const std::string name);
        virtual void setHandlePosition(const Vec2r& position);
        virtual void setSize(const Vec2r& size);
        virtual void setWidth(real width);
        virtual void setHeight(real height);
        virtual void setTexOrigin(const Vec2r& origin);
        virtual void setTexExtents(const Vec2r& extents);
        virtual void setColorFilter(const Vec4r& color);

    private:
        std::string _imageName;
        std::vector<Vec2r> _cornersPositions;
        std::vector<Vec2r> _cornersTexCoords;
        Vec4r _colorFilter;
    };



    // IMPLEMENTATION //
    inline const std::vector<Vec2r>& ImageHud::cornersPositions() const
    {
        return _cornersPositions;
    }

    inline const std::vector<Vec2r>& ImageHud::cornersTexCoords() const
    {
        return _cornersTexCoords;
    }

    inline Vec4r ImageHud::colorFilter() const
    {
        return _colorFilter;
    }

    inline void ImageHud::setColorFilter(const Vec4r &color)
    {
        _colorFilter = color;
    }
}

#endif // PROPROOM2D_IMAGEHUD_H
