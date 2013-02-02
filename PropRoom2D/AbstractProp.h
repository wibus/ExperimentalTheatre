#ifndef PROPROOM2D_ABSTRACTPROP_H
#define PROPROOM2D_ABSTRACTPROP_H

#include "libPropRoom2D_global.h"


namespace prop2
{
    class PROP2D_EXPORT AbstractProp
    {
    protected:
        AbstractProp(PropType::Enum propType);

    public:
        virtual ~AbstractProp();

        // Getters
        PropId   id() const;
        PropType::Enum propType() const;
        bool     isVisible() const;

        // Setters
        void setIsVisible(bool isVisible);

    private:
        static PropId _assigneId_();
        static PropId _nextId_;

        PropId         _id;
        PropType::Enum _propType;
        bool           _isVisible;
    };



    // IMPLEMENTATION //
    inline PropId AbstractProp::id() const
    {
        return _id;
    }

    inline PropType::Enum AbstractProp::propType() const
    {
        return _propType;
    }

    inline bool AbstractProp::isVisible() const
    {
        return _isVisible;
    }

    inline void AbstractProp::setIsVisible(bool isVisible)
    {
        _isVisible = isVisible;
    }

    inline PropId AbstractProp::_assigneId_()
    {
        return ++_nextId_;
    }
}

#endif // PROPROOM2D_ABSTRACTPROP_H
