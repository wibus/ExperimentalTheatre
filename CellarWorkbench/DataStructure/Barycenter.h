#ifndef CELLARWORKBENCH_BARYCENTER_H
#define CELLARWORKBENCH_BARYCENTER_H

#include "libCellarWorkbench_global.h"
#include "Vector3D.h"


namespace cellar
{
    class CELLAR_EXPORT Barycenter
    {
    public:
        Barycenter(const Vector3D<float>& center = Vector3D<float>(0.0f, 0.0f, 0.0f),
                   float mass=0.0f);

        // Geters
        const Vector3D<float> center() const;
        float mass() const;

        // Seters
        void setCenter(const Vector3D<float>& center);
        void setMass(float mass);

        // Operators
              Barycenter& operator()(const Vector3D<float>& center, float mass);
        const Barycenter  operator+ (const Barycenter& barycenter) const;
              Barycenter& operator+=(const Barycenter& barycenter);
        const Barycenter  operator- (const Barycenter& barycenter) const;
              Barycenter& operator-=(const Barycenter& barycenter);

    private :
        Vector3D<float> _center;
        float _mass;
    };



    // IMPLEMENTATION //
    inline Barycenter::Barycenter(const Vector3D<float>& center, float mass=0.0f)
        : _center(center),
          _mass(mass) {}
    //Geters
    inline Vector3D<float> Barycenter::center() const {return _center;}
    inline float Barycenter::mass() const {return _mass;}
    //Seters
    inline void Barycenter::setCenter(const Vector3D<float>& center) {_center = center;}
    inline void Barycenter::setMass(float mass) {_mass = mass;}
    //Operators
    inline Barycenter& Barycenter::operator()(const Vector3D<float>& center, float mass)
    {
        _center = center;
        _mass = mass;
        return *this;
    }
    inline const Barycenter Barycenter::operator+ (const Barycenter& barycenter) const
    {
        Vector3D<float> newCenter =
                (_center*_mass + barycenter._center*barycenter._mass) /
                (_mass+barycenter._mass);
        float newMass = _mass+barycenter._mass;

        return Barycenter(newCenter, newMass);
    }
    inline Barycenter& Barycenter::operator+=(const Barycenter& barycenter)
    {
        Vector3D<float> newCenter =
                (_center*_mass + barycenter._center*barycenter._mass) /
                (_mass+barycenter._mass);
        float newMass = _mass+barycenter._mass;

        return (*this)(newCenter, newMass);
    }
    inline const Barycenter Barycenter::operator-(const Barycenter& barycenter) const
    {
        Vector3D<float> newCenter =
                (_center*_mass - barycenter._center*barycenter._mass) /
                (_mass-barycenter._mass);
        float newMass = _mass-barycenter._mass;

        return Barycenter(newCenter, newMass);
    }
    inline Barycenter& Barycenter::operator-=(const Barycenter& barycenter)
    {
        Vector3D<float> newCenter =
                (_center*_mass - barycenter._center*barycenter._mass) /
                (_mass-barycenter._mass);
        float newMass = _mass-barycenter._mass;

        return (*this)(newCenter, newMass);
    }
}

#endif // CELLARWORKBENCH_BARYCENTER_H
