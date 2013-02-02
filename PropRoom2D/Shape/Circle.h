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
        const std::shared_ptr<CircleCostume>& costume() const;
        void setCostume(const std::shared_ptr<CircleCostume>& costume);


        // Shape properties
        real radius() const;
        virtual void setRadius(const real& radius);
        virtual void setCenter(const Vec2r& position);


        // Tests
        virtual bool contains(const Vec2r& point) const;
        virtual bool intersects(const Segment2Dr& line) const;


    protected:
        // Computations
        virtual void updateTranformMatrix();
        virtual void updatePerimeter();
        virtual void updateArea();
        virtual void updateInertia();


        // Attributes
        std::shared_ptr<CircleCostume> _costume;
        real _radius;
    };



    // IMPLEMENTATION //
    inline const std::shared_ptr<CircleCostume>& Circle::costume() const
    {
        return _costume;
    }

    inline void Circle::setCostume(const std::shared_ptr<CircleCostume>& costume)
    {
        _costume = costume;
    }

    inline real Circle::radius() const
    {
        return _radius;
    }
}

#endif // PROPROOM2D_CIRCLE_H
