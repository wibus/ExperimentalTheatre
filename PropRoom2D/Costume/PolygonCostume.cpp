#include "PolygonCostume.h"

#include <cassert>


namespace prop2
{
    PolygonCostume::PolygonCostume(int nbVertices) :
        _verticesColors(nbVertices, Vec4r(1, 1, 1, 1)),
        _verticesTexCoords(nbVertices, Vec2r(0, 0))
    {
    }

    PolygonCostume::~PolygonCostume()
    {
    }

    void PolygonCostume::setVertexColor(int index, const Vec4r& color)
    {
        assert(0 <= index && index < (int)_verticesColors.size());
        _verticesColors[index] = color;
    }

    void PolygonCostume::setVerticesColors(const std::vector<Vec4r>& colors)
    {
        assert(colors.size() == _verticesColors.size());
        _verticesColors = colors;
    }

    void PolygonCostume::setVertexTexCoord(int index, const Vec2r& texCoord)
    {
        assert(0 <= index && index < (int)_verticesTexCoords.size());
        _verticesTexCoords[index] = texCoord;
    }

    void PolygonCostume::setVerticesTexCoords(const std::vector<Vec2r>& texCoords)
    {
        assert(texCoords.size() == _verticesTexCoords.size());
        _verticesTexCoords = texCoords;
    }
}
