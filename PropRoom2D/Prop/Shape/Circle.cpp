#include "Circle.h"

#include <GLM/gtc/matrix_transform.hpp>


using namespace cellar;

#include "../Costume/CircleCostume.h"
#include "../Hardware/Hardware.h"


namespace prop2
{
    Circle::Circle() :
        AbstractShape(EPropType::CIRCLE),
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

    double Circle::radius() const
    {
        return _radius;
    }

    void Circle::setRadius(const double& radius)
    {
        _radius = radius;

        // Update cached attributes
        updateTransformMatrix();
        updateInertia();
    }

    glm::dvec2 Circle::center() const
    {
        return _centroid;
    }

    void Circle::setCenter(const glm::dvec2& position)
    {
       setCentroid(position);
    }

    bool Circle::contains(const glm::dvec2& point) const
    {
        return glm::length(_centroid - point) < _radius;
    }

    glm::dvec2 Circle::nearestSurface(const glm::dvec2& point) const
    {
        glm::dvec2 direction = point - _centroid;
        double distance = glm::length(direction);
        return normalize(direction) * (_radius - distance);
    }

    double Circle::computeArea() const
    {
        return glm::pi<double>() * _radius * _radius;
    }

    void Circle::updateTransformMatrix()
    {
        double r = _radius;
        double c = glm::cos(_angle);
        double s = glm::sin(_angle);

        _tranformMatrix = glm::dmat3();
        _tranformMatrix[0][0] = c*r;  _tranformMatrix[1][0] = -s*r; _tranformMatrix[2][0] = _centroid.x;
        _tranformMatrix[0][1] = s*r;  _tranformMatrix[1][1] = c*r;  _tranformMatrix[2][1] = _centroid.y;
        _tranformMatrix[0][2] = 0.0f; _tranformMatrix[1][2] = 0.0f; _tranformMatrix[2][2] = 1.0f;
    }

    void Circle::updateInertia()
    {
        if(_material)
        {
            _inverseMass = 1.0 / (_material->density() * computeArea());
            _inverseMomentOfInertia = (2 * _inverseMass) / (_radius * _radius);
        }
        else
        {
            _inverseMass = 0;
            _inverseMomentOfInertia = 0;
        }
    }
}
