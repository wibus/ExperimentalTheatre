#ifndef PROPROOM2D_CIRCLECOSTUME_H
#define PROPROOM2D_CIRCLECOSTUME_H

#include "libPropRoom2D_global.h"
#include "AbstractCostume.h"


namespace prop2
{
    class PROP2D_EXPORT CircleCostume : public AbstractCostume
    {
    public:
        CircleCostume();
        virtual ~CircleCostume();

        // Getters
        real textureRadius() const;
        Vec2r textureCenter() const;

        // Setters
        virtual void setTextureRadius(real radius);
        virtual void setTextureCenter(const Vec2r center);

    private:
        real _textureRadius;
        Vec2r _textureCenter;
    };



    // IMPLEMENTATION //
    inline real CircleCostume::textureRadius() const
    {
        return _textureRadius;
    }

    inline Vec2r CircleCostume::textureCenter() const
    {
        return _textureCenter;
    }
}

#endif // PROPROOM2D_CIRCLECOSTUME_H
