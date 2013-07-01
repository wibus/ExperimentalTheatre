#ifndef PROPROOM2D_ABSTRACTCOSTUME_H
#define PROPROOM2D_ABSTRACTCOSTUME_H

#include "libPropRoom2D_global.h"

#include <string>


namespace prop2
{
    class PROP2D_EXPORT AbstractCostume
    {
    protected:
        AbstractCostume();

    public:
        virtual ~AbstractCostume();

        // Getters
        real depth() const;
        Vec4r colorFilter() const;
        std::string textureName() const;

        // Setters
        virtual void setDepth(real depth);
        virtual void setColorFilter(const Vec4r& filter);
        virtual void setTextureName(const std::string& fileName);

    private:
        real _depth;
        Vec4r _colorFilter;
        std::string _textureName;
    };



    // IMPLEMENTATION //

    inline real AbstractCostume::depth() const
    {
        return _depth;
    }

    inline Vec4r AbstractCostume::colorFilter() const
    {
        return _colorFilter;
    }

    inline std::string AbstractCostume::textureName() const
    {
        return _textureName;
    }
}

#endif // PROPROOM2D_ABSTRACTCOSTUME_H
