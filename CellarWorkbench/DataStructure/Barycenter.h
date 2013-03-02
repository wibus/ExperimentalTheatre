#ifndef CELLARWORKBENCH_BARYCENTER_H
#define CELLARWORKBENCH_BARYCENTER_H

#include "libCellarWorkbench_global.h"
#include "Vector.h"


namespace cellar
{
    class CELLAR_EXPORT Barycenter
    {
    public:
        Barycenter(const Vec3f& center = Vec3f(0.0f, 0.0f, 0.0f),
                   float mass=0.0f);

        // Geters
        const Vec3f center() const;
        float mass() const;

        // Seters
        void setCenter(const Vec3f& center);
        void setMass(float mass);

        // Operators
              Barycenter& operator()(const Vec3f& center, float mass);
        const Barycenter  operator+ (const Barycenter& barycenter) const;
              Barycenter& operator+=(const Barycenter& barycenter);
        const Barycenter  operator- (const Barycenter& barycenter) const;
              Barycenter& operator-=(const Barycenter& barycenter);

    private :
        Vec3f _center;
        float _mass;
    };



    // IMPLEMENTATION //
    inline Barycenter::Barycenter(const Vec3f& center, float mass=0.0f)
        : _center(center),
          _mass(mass) {}
    //Geters
    inline Vec3f Barycenter::center() const {return _center;}
    inline float Barycenter::mass() const {return _mass;}
    //Seters
    inline void Barycenter::setCenter(const Vec3f& center) {_center = center;}
    inline void Barycenter::setMass(float mass) {_mass = mass;}
    //Operators
    inline Barycenter& Barycenter::operator()(const Vec3f& center, float mass)
    {
        _center = center;
        _mass = mass;
        return *this;
    }
    inline const Barycenter Barycenter::operator+ (const Barycenter& barycenter) const
    {
        Vec3f newCenter =
                (_center*_mass + barycenter._center*barycenter._mass) /
                (_mass+barycenter._mass);
        float newMass = _mass+barycenter._mass;

        return Barycenter(newCenter, newMass);
    }
    inline Barycenter& Barycenter::operator+=(const Barycenter& barycenter)
    {
        Vec3f newCenter =
                (_center*_mass + barycenter._center*barycenter._mass) /
                (_mass+barycenter._mass);
        float newMass = _mass+barycenter._mass;

        return (*this)(newCenter, newMass);
    }
    inline const Barycenter Barycenter::operator-(const Barycenter& barycenter) const
    {
        Vec3f newCenter =
                (_center*_mass - barycenter._center*barycenter._mass) /
                (_mass-barycenter._mass);
        float newMass = _mass-barycenter._mass;

        return Barycenter(newCenter, newMass);
    }
    inline Barycenter& Barycenter::operator-=(const Barycenter& barycenter)
    {
        Vec3f newCenter =
                (_center*_mass - barycenter._center*barycenter._mass) /
                (_mass-barycenter._mass);
        float newMass = _mass-barycenter._mass;

        return (*this)(newCenter, newMass);
    }
}

#endif // CELLARWORKBENCH_BARYCENTER_H
