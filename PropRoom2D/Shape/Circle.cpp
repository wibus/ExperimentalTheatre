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
        return (centroid() - point).length() < _radius;
    }

    Vec2r Circle::nearestSurface(const Vec2r& point) const
    {
        Vec2r direction = point - centroid();
        real distance = direction.length();
        return direction.normalize() * (radius() - distance);
    }

    void Circle::updateTranformMatrix()
    {
        _tranformMatrix.loadIdentity();
        _tranformMatrix *= cellar::translate(_centroid.x(), _centroid.y());
        _tranformMatrix *= cellar::scale(_radius, _radius, real(1));        
        _tranformMatrix *= cellar::rotate(_angle);
    }

    void Circle::updatePerimeter()
    {
        _perimeter = 2 * static_cast<real>(PI) * _radius;
    }

    void Circle::updateArea()
    {
        _area = static_cast<real>(PI) * _radius * _radius;
    }

    void Circle::updateInertia()
    {
        if(_density == INFINITE_DENSITY ||
           _bodyType != BodyType::DYNAMIC)
        {
            _inverseMass = INFINITE_INERTIA;
            _inverseMomentOfInertia = INFINITE_INERTIA;
        }
        else
        {
            _inverseMass = real(1.0) / (_density * _area);
            _inverseMomentOfInertia = (2 * _inverseMass) / (_radius * _radius);
        }
    }
}
