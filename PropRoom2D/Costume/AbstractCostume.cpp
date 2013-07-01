#include "AbstractCostume.h"


namespace prop2
{
    AbstractCostume::AbstractCostume() :
        _depth(real(0.0)),
        _colorFilter(real(1.0), real(1.0), real(1.0), real(1.0)),
        _textureName("")
    {
    }

    AbstractCostume::~AbstractCostume()
    {
    }

    void AbstractCostume::setDepth(real depth)
    {
        _depth = depth;
    }

    void AbstractCostume::setColorFilter(const Vec4r& filter)
    {
        _colorFilter = filter;
    }

    void AbstractCostume::setTextureName(const std::string& fileName)
    {
        _textureName = fileName;
    }
}
