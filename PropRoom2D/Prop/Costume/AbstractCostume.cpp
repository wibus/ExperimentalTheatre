#include "AbstractCostume.h"


namespace prop2
{
    AbstractCostume::AbstractCostume() :
        _depth(0.0),
        _colorFilter(1.0, 1.0, 1.0, 1.0),
        _textureName("")
    {
    }

    AbstractCostume::~AbstractCostume()
    {
    }

    void AbstractCostume::setDepth(double depth)
    {
        _depth = depth;
    }

    void AbstractCostume::setColorFilter(const glm::dvec4& filter)
    {
        _colorFilter = filter;
    }

    void AbstractCostume::setTextureName(const std::string& fileName)
    {
        _textureName = fileName;
    }
}
