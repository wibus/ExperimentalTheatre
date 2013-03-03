#include "Circle.h"
#include "Costume/CircleCostume.h"

#include <Misc/CellarUtils.h>
using namespace cellar;


namespace prop2
{
    Circle::Circle() :
        AbstractShape(PropType::CIRCLE),
        _costume(),
        _radius(1.0)
    {
    }

    Circle::~Circle()
    {
    }

    const AbstractCostume& Circle::abstractCostume() const
    {
        return *_costume;
    }

    void Circle::setRadius(const real& radius)
    {
        _radius = radius;

        // Update cached attributes
        updateTranformMatrix();
        updatePerimeter();
        updateArea();
        updateInertia();
    }

    void Circle::setCenter(const Vec2r& position)
    {
        setCentroid(position);
    }

    bool Circle::contains(const Vec2r& point) const
    {
        return (_centroid - point).length() < _radius;
    }

    bool Circle::intersects(const Segment2Dr& line) const
    {
        return line.pointMinimalDistance(_centroid) < _radius;
    }

    void Circle::updateTranformMatrix()
    {
        _tranformMatrix.loadIdentity();
        _tranformMatrix *= cellar::scale(_radius, _radius, real(1));
        _tranformMatrix *= cellar::translate(_centroid.x(), _centroid.y());
        _tranformMatrix *= cellar::rotate(_angle);
    }

    void Circle::updatePerimeter()
    {
        _perimeter = 2 * PI * _radius;
    }

    void Circle::updateArea()
    {
        _area = PI * _radius * _radius;
    }

    void Circle::updateInertia()
    {
        _mass = _density * _area;
        _momentOfInertia = real(0.5) * _mass * _radius * _radius;
    }
}
