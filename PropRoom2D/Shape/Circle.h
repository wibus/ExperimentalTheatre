#ifndef PROPROOM2D_CIRCLE_H
#define PROPROOM2D_CIRCLE_H

#include <memory>

#include "libPropRoom2D_global.h"
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
        virtual real radius() const;
        virtual void setRadius(const real& radius);

        // Center (centroid
        virtual Vec2r center() const;
        virtual void setCenter(const Vec2r& position);

        // Tests
        virtual bool contains(const Vec2r& point) const;
        virtual Vec2r nearestSurface(const Vec2r& point) const;

        // Area
        virtual real computeArea() const;


    protected:
        // Cached attributes update
        virtual void updateTranformMatrix();
        virtual void updateInertia();


        // Attributes
        std::shared_ptr<CircleCostume> _costume;
        real _radius;
    };
}

#endif // PROPROOM2D_CIRCLE_H
