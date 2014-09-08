#include "Barycenter.h"

namespace cellar
{

Barycenter::Barycenter() :
    _center(),
    _mass(0.0)
{
}

Barycenter::Barycenter(const Vec3d& center, double mass) :
    _center(center),
    _mass(mass)
{
}

Barycenter& Barycenter::operator()(const Vec3d& center, double mass)
{
    _center = center;
    _mass = mass;
    return *this;
}

Barycenter Barycenter::operator+ (const Barycenter& barycenter) const
{
    Barycenter result;
    result._mass = _mass + barycenter._mass;
    result._center = (_center*_mass + barycenter._center*barycenter._mass) /
                     result._mass;
    return result;
}

Barycenter& Barycenter::operator+=(const Barycenter& barycenter)
{
    _center = (_center*_mass + barycenter._center*barycenter._mass) /
              (_mass + barycenter._mass);
    _mass = _mass + barycenter._mass;
    return *this;
}

Barycenter Barycenter::operator-(const Barycenter& barycenter) const
{
    Barycenter result;
    if(barycenter._mass < _mass)
    {
        result._mass = _mass - barycenter._mass;
        result._center = (_center*_mass - barycenter._center*barycenter._mass) /
                         result._mass;
    }
    // else {Use default values}
    return *this;
}

Barycenter& Barycenter::operator-=(const Barycenter& barycenter)
{
    if(barycenter._mass < _mass)
    {
        _center = (_center*_mass - barycenter._center*barycenter._mass) /
                  (_mass - barycenter._mass);
        _mass = _mass - barycenter._mass;
    }
    else
    {
        _center(0.0, 0.0, 0.0);
        _mass = 0.0;
    }
    return *this;
}

}
