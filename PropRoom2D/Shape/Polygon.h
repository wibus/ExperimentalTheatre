#ifndef PROPROOM2D_POLYGON_H
#define PROPROOM2D_POLYGON_H

#include <vector>

#include <Geometry/Segment2D.h>

#include "libPropRoom2D_global.h"
#include "AbstractShape.h"

namespace prop2
{
class PolygonCostume;


    class PROP2D_EXPORT Polygon : public AbstractShape
    {
    public:
        Polygon(int nbVertices);
        virtual ~Polygon();


        // Costume
        virtual const AbstractCostume& abstractCostume() const;
        const std::shared_ptr<PolygonCostume>& costume() const;
        void setCostume(const std::shared_ptr<PolygonCostume>& costume);


        // Vertices
        int nbVertices() const;
        const std::vector<Vec2r>& relVertices() const;
        const std::vector<Vec2r>& absVertices() const;
        const std::vector< Segment2Dr >& outline() const;
        virtual void setVertices(const std::vector<Vec2r>& bruteVertices);
        virtual void moveVertexAt(int cIdx, const Vec2r& position);


        // Tests
        virtual bool contains(const Vec2r& point) const;
        virtual bool intersects(const Segment2Dr& line) const;


    protected:
        // Computations
        virtual void updateAbsVerticesAndOutline();
        virtual void updateTranformMatrix();
        virtual void updatePerimeter();
        virtual void updateArea();
        virtual void updateInertia();

        static Vec2r evaluateCentroid(const std::vector<Vec2r>& vertices);
        static std::vector<Segment2Dr> evaluateOutline(const std::vector<Vec2r>& vertices);


        // Attributes
        std::shared_ptr<PolygonCostume> _costume;
        std::vector<Vec2r> _relVertices;
        std::vector<Vec2r> _absVertices;
        std::vector< Segment2Dr > _outline;
    };



    // IMPLEMENTATION //
    inline const std::shared_ptr<PolygonCostume>& Polygon::costume() const
    {
        return _costume;
    }

    inline int Polygon::nbVertices() const
    {
        return _relVertices.size();
    }

    inline const std::vector<Vec2r>& Polygon::relVertices() const
    {
        return _relVertices;
    }

    inline const std::vector<Vec2r>& Polygon::absVertices() const
    {
        return _absVertices;
    }

    inline const std::vector<Segment2Dr>& Polygon::outline() const
    {
        return _outline;
    }
}

#endif // PROPROOM2D_POLYGON_H
