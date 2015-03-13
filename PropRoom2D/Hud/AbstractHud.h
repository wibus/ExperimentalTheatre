#ifndef PROPROOM2D_ABSTRACTHUD_H
#define PROPROOM2D_ABSTRACTHUD_H

#include <GLM/glm.hpp>

#include "../AbstractProp.h"


namespace prop2
{
    class PROP2D_EXPORT AbstractHud : public AbstractProp
    {
    protected:
        AbstractHud(EPropType propType);

    public:
        virtual ~AbstractHud();

        // Getters
        virtual double width() const =0;
        virtual double height() const =0;
        virtual glm::dvec2 handlePosition() const = 0;
        EHorizontalAnchor horizontalAnchor() const;
        EVerticalAnchor   verticalAnchor() const;

        // Setters
        virtual void setHeight(double height) =0;
        virtual void setHandlePosition(const glm::dvec2& position) = 0;
        void setHorizontalAnchor(const EHorizontalAnchor& anchor);
        void setVerticalAnchor(const EVerticalAnchor& anchor);

    protected:
        EHorizontalAnchor _horizontalAnchor;
        EVerticalAnchor   _verticalAnchor;
    };



    // IMPLEMENTATION //
    inline EHorizontalAnchor AbstractHud::horizontalAnchor() const
    {
        return _horizontalAnchor;
    }

    inline EVerticalAnchor AbstractHud::verticalAnchor() const
    {
        return _verticalAnchor;
    }

    inline void AbstractHud::setHorizontalAnchor(const EHorizontalAnchor& anchor)
    {
        _horizontalAnchor = anchor;
    }

    inline void AbstractHud::setVerticalAnchor(const EVerticalAnchor& anchor)
    {
        _verticalAnchor = anchor;
    }
}

#endif // PROPROOM2D_ABSTRACTHUD_H
