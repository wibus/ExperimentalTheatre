#include "PolygonCostume.h"

#include <cassert>


namespace prop2
{
    PolygonCostume::PolygonCostume(int nbVertices) :
        _verticesColors(nbVertices, glm::dvec4(1, 1, 1, 1)),
        _verticesTexCoords(nbVertices, glm::dvec2(0, 0))
    {
        if(nbVertices == 4)
        {
            // Handle the most frequent case
            _verticesTexCoords[0] = glm::dvec2(0.0, 0.0);
            _verticesTexCoords[1] = glm::dvec2(1.0, 0.0);
            _verticesTexCoords[2] = glm::dvec2(1.0, 1.0);
            _verticesTexCoords[3] = glm::dvec2(0.0, 1.0);
        }
    }

    PolygonCostume::~PolygonCostume()
    {
    }

    void PolygonCostume::setVertexColor(int index, const glm::dvec4& color)
    {
        assert(0 <= index && index < (int)_verticesColors.size());
        _verticesColors[index] = color;
    }

    void PolygonCostume::setVerticesColors(const std::vector<glm::dvec4>& colors)
    {
        assert(colors.size() == _verticesColors.size());
        _verticesColors = colors;
    }

    void PolygonCostume::setVertexTexCoord(int index, const glm::dvec2& texCoord)
    {
        assert(0 <= index && index < (int)_verticesTexCoords.size());
        _verticesTexCoords[index] = texCoord;
    }

    void PolygonCostume::setVerticesTexCoords(const std::vector<glm::dvec2>& texCoords)
    {
        assert(texCoords.size() == _verticesTexCoords.size());
        _verticesTexCoords = texCoords;
    }
}
