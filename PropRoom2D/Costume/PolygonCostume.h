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
        std::vector<Vec4r> verticesColors() const;
        std::vector<Vec2r> verticesTexCoords() const;

        // Setters
        virtual void setVertexColor(int index, const Vec4r& color);
        virtual void setVerticesColors(const std::vector<Vec4r>& colors);
        virtual void setVertexTexCoord(int index, const Vec2r& texCoord);
        virtual void setVerticesTexCoords(const std::vector<Vec2r>& texCoords);

    private:
        std::vector<Vec4r> _verticesColors;
        std::vector<Vec2r> _verticesTexCoords;
    };



    // IMPLEMENTATION //
    inline std::vector<Vec4r> PolygonCostume::verticesColors() const
    {
        return _verticesColors;
    }

    inline std::vector<Vec2r> PolygonCostume::verticesTexCoords() const
    {
        return _verticesTexCoords;
    }
}

#endif // PROPROOM2D_POLYGONCOSTUME_H
