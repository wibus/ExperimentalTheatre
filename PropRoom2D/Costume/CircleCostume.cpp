#include "CircleCostume.h"


namespace prop2
{
    CircleCostume::CircleCostume() :
        _textureRadius(real(1.0)),
        _textureCenter(real(0.5), real(0.5))
    {
    }

    CircleCostume::~CircleCostume()
    {
    }

    void CircleCostume::setTextureRadius(real radius)
    {
        _textureRadius = radius;
    }

    void CircleCostume::setTextureCenter(const Vec2r center)
    {
        _textureCenter = center;
    }
}
