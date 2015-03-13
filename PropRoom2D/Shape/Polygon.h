#ifndef PROPROOM2D_POLYGON_H
#define PROPROOM2D_POLYGON_H

#include <vector>

#include <CellarWorkbench/Geometry/Segment2D.h>

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
        virtual const std::vector<glm::dvec2>& relVertices() const;
        virtual const std::vector<cellar::Segment2D>& outline() const;
        virtual void setVertices(const std::vector<glm::dvec2>& absolutePositions);
        virtual void moveVertexAt(int cIdx, const glm::dvec2& position);

        // Tests
        virtual bool contains(const glm::dvec2& point) const;
        virtual glm::dvec2 nearestSurface(const glm::dvec2& point) const;

        // Area
        virtual double computeArea() const;

        // Polygon type
        virtual bool isConcave() const;

        // Polygon vertices generation
        static std::vector<glm::vec2> rectangle(double width, double height);
        static std::vector<glm::vec2> regularPolygon(double radius, int nbSides);

    protected:
        // Computations
        virtual void updateTransformMatrix();
        virtual void updateInertia();

        // Helper methods
        static glm::dvec2 evaluateCentroid(const std::vector<glm::dvec2>& vertices);


        // Attributes
        std::shared_ptr<PolygonCostume> _costume;
        std::vector<glm::dvec2> _relVertices;
        std::vector<cellar::Segment2D> _outline;
        bool _isConcave;
    };
}

#endif // PROPROOM2D_POLYGON_H
