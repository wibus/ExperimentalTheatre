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
        virtual const std::shared_ptr<PolygonCostume>& costume() const;
        virtual void setCostume(const std::shared_ptr<PolygonCostume>& costume);

        // Vertices
        int nbVertices() const;
        virtual const std::vector<Vec2r>& relVertices() const;
        virtual const std::vector< Segment2Dr >& outline() const;
        virtual void setVertices(const std::vector<Vec2r>& absolutePositions);
        virtual void moveVertexAt(int cIdx, const Vec2r& position);

        // Tests
        virtual bool contains(const Vec2r& point) const;
        virtual Vec2r nearestSurface(const Vec2r& point) const;

        // Area
        virtual real computeArea() const;


    protected:
        // Computations
        virtual void updateTranformMatrix();
        virtual void updateInertia();

        // Helper methods
        static Vec2r evaluateCentroid(const std::vector<Vec2r>& vertices);


        // Attributes
        std::shared_ptr<PolygonCostume> _costume;
        std::vector<Vec2r> _relVertices;
        std::vector< Segment2Dr > _outline;
    };
}

#endif // PROPROOM2D_POLYGON_H
