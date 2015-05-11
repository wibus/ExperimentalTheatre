#ifndef PROPROOM2D_POLYGONCOSTUME_H
#define PROPROOM2D_POLYGONCOSTUME_H

#include <vector>

#include "AbstractCostume.h"


namespace prop2
{
    class PROP2D_EXPORT PolygonCostume : public AbstractCostume
    {
    public:
        PolygonCostume(int nbVertices);
        virtual ~PolygonCostume();

        // Getters
        std::vector<glm::dvec4> verticesColors() const;
        std::vector<glm::dvec2> verticesTexCoords() const;

        // Setters
        virtual void setVertexColor(int index, const glm::dvec4& color);
        virtual void setVerticesColors(const std::vector<glm::dvec4>& colors);
        virtual void setVertexTexCoord(int index, const glm::dvec2& texCoord);
        virtual void setVerticesTexCoords(const std::vector<glm::dvec2>& texCoords);

    private:
        std::vector<glm::dvec4> _verticesColors;
        std::vector<glm::dvec2> _verticesTexCoords;
    };



    // IMPLEMENTATION //
    inline std::vector<glm::dvec4> PolygonCostume::verticesColors() const
    {
        return _verticesColors;
    }

    inline std::vector<glm::dvec2> PolygonCostume::verticesTexCoords() const
    {
        return _verticesTexCoords;
    }
}

#endif // PROPROOM2D_POLYGONCOSTUME_H
