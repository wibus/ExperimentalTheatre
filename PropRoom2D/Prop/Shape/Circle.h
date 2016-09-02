#ifndef PROPROOM2D_CIRCLE_H
#define PROPROOM2D_CIRCLE_H

#include <memory>

#include "AbstractShape.h"


namespace prop2
{
class CircleCostume;


    class PROP2D_EXPORT Circle : public AbstractShape
    {
    public:
        Circle();
        virtual ~Circle();


        // Costume
        virtual const AbstractCostume& abstractCostume() const;
        virtual const std::shared_ptr<CircleCostume>& costume() const;
        virtual void setCostume(const std::shared_ptr<CircleCostume>& costume);

        // Radius
        virtual double radius() const;
        virtual void setRadius(const double& radius);

        // Center (centroid)
        virtual glm::dvec2 center() const;
        virtual void setCenter(const glm::dvec2& position);

        // Tests
        virtual bool contains(const glm::dvec2& point) const;
        virtual glm::dvec2 nearestSurface(const glm::dvec2& point) const;

        // Area
        virtual double computeArea() const;


    protected:
        // Cached attributes update
        virtual void updateTransformMatrix();
        virtual void updateInertia();


        // Attributes
        std::shared_ptr<CircleCostume> _costume;
        double _radius;
    };
}

#endif // PROPROOM2D_CIRCLE_H
