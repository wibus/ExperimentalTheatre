#include "Circle.h"
#include "Costume/CircleCostume.h"
#include "Material/Material.h"

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

    const std::shared_ptr<CircleCostume>& Circle::costume() const
    {
        return _costume;
    }

    void Circle::setCostume(const std::shared_ptr<CircleCostume>& costume)
    {
        _costume = costume;
    }

    real Circle::radius() const
    {
        return _radius;
    }

    void Circle::setRadius(const real& radius)
    {
        _radius = radius;

        // Update cached attributes
        updateTranformMatrix();
        updateInertia();
    }

    Vec2r Circle::center() const
    {
        return _centroid;
    }

    void Circle::setCenter(const Vec2r& position)
    {
       setCentroid(position);
    }

    bool Circle::contains(const Vec2r& point) const
    {
        return (_centroid - point).length() < _radius;
    }

    Vec2r Circle::nearestSurface(const Vec2r& point) const
    {
        Vec2r direction = point - _centroid;
        real distance = direction.length();
        return direction.normalize() * (_radius - distance);
    }

    real Circle::computeArea() const
    {
        return static_cast<real>(PI) * _radius * _radius;
    }

    void Circle::updateTranformMatrix()
    {
        _tranformMatrix.loadIdentity();
        _tranformMatrix *= cellar::translate(_centroid);
        _tranformMatrix *= cellar::scale(_radius, _radius);
        _tranformMatrix *= cellar::rotate(_angle);
    }

    void Circle::updateInertia()
    {
        if(_material)
        {
            _inverseMass = real(1.0) / (_material->density() * computeArea());
            _inverseMomentOfInertia = (2 * _inverseMass) / (_radius * _radius);
        }
        else
        {
            _inverseMass = real(0);
            _inverseMomentOfInertia = real(0);
        }
    }
}
