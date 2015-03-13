#include "CircleCostume.h"


namespace prop2
{
    CircleCostume::CircleCostume() :
        _textureRadius(1.0),
        _textureCenter(0.5, 0.5)
    {
    }

    CircleCostume::~CircleCostume()
    {
    }

    void CircleCostume::setTextureRadius(double radius)
    {
        _textureRadius = radius;
    }

    void CircleCostume::setTextureCenter(const glm::dvec2 center)
    {
        _textureCenter = center;
    }
}
