#ifndef PROPROOM2D_ABSTRACTHUD_H
#define PROPROOM2D_ABSTRACTHUD_H

#include "libPropRoom2D_global.h"
#include "AbstractProp.h"


namespace prop2
{
    class PROP2D_EXPORT AbstractHud : public AbstractProp
    {
    protected:
        AbstractHud(PropType::Enum propType);

    public:
        virtual ~AbstractHud();

        // Getters
        virtual real width() const =0;
        virtual real height() const =0;
        virtual Vec2r handlePosition() const = 0;
        HorizontalAnchor::Enum horizontalAnchor() const;
        VerticalAnchor::Enum   verticalAnchor() const;

        // Setters
        virtual void setHeight(real height) =0;
        virtual void setHandlePosition(const Vec2r& position) = 0;
        void setHorizontalAnchor(const HorizontalAnchor::Enum& anchor);
        void setVerticalAnchor(const VerticalAnchor::Enum& anchor);

    protected:
        HorizontalAnchor::Enum _horizontalAnchor;
        VerticalAnchor::Enum _verticalAnchor;
    };



    // IMPLEMENTATION //
    inline HorizontalAnchor::Enum AbstractHud::horizontalAnchor() const
    {
        return _horizontalAnchor;
    }

    inline VerticalAnchor::Enum AbstractHud::verticalAnchor() const
    {
        return _verticalAnchor;
    }

    inline void AbstractHud::setHorizontalAnchor(const HorizontalAnchor::Enum& anchor)
    {
        _horizontalAnchor = anchor;
    }

    inline void AbstractHud::setVerticalAnchor(const VerticalAnchor::Enum& anchor)
    {
        _verticalAnchor = anchor;
    }
}

#endif // PROPROOM2D_ABSTRACTHUD_H
