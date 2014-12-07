#include "AbstractCostume.h"


namespace prop3
{
    AbstractCostume::AbstractCostume() :
        _colorFilter(1.0, 1.0, 1.0, 1.0),
        _textureName("")
    {
    }

    AbstractCostume::~AbstractCostume()
    {
    }

    void AbstractCostume::setColorFilter(const glm::vec4& filter)
    {
        _colorFilter = filter;
    }

    void AbstractCostume::setTextureName(const std::string& fileName)
    {
        _textureName = fileName;
    }
}
