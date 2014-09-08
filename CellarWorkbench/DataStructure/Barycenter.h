#ifndef CELLARWORKBENCH_BARYCENTER_H
#define CELLARWORKBENCH_BARYCENTER_H

#include "libCellarWorkbench_global.h"
#include "Vector.h"


namespace cellar
{

/// To compute the center of mass of a multibody object
class CELLAR_EXPORT Barycenter
{
public:
    /// Default constructor
    Barycenter();

    /// Constructor
    /// \param[in] center Position of the center of mass
    /// \param[in] mass Total mass at that point
    Barycenter(const Vec3d& center, double mass);

    /// Getter for the position of the center of mass
    /// \return Position of the center of mass
    const Vec3d& getCenter() const;

    /// Getter for the total mass
    /// \return Total mass
    double getMass() const;

    /// Setter for the position of the position of the center of mass
    /// \param[in] center Position of the center of mass
    void setCenter(const Vec3d& getCenter);

    /// Setter for the total mass
    /// \param[in] mass Total of mass
    void setMass(double getMass);

    /// Parenthesis operator overloaded to set both 'center' and 'mass'.
    /// \param[in] center Position of the center of mass
    /// \param[in] mass Total of mass
    /// \return A reference to this object
    Barycenter& operator()(const Vec3d& center, double mass);

    /// Addition operator overloaded to compute the union of two barycenter
    /// \param[in] barycenter Barycenter to add to the actual barycenter
    /// \return The addition of the two barycenters
    Barycenter  operator+ (const Barycenter& barycenter) const;

    /// Addition assignement operator overloaded to add a barycenter to this
    /// \param[in] barycenter Barycenter to add to the actual barycenter
    /// \return This augmented barycenter
    Barycenter& operator+=(const Barycenter& barycenter);

    /// Subtraction operator overloaded to subtract a part of this barycenter
    /// \param[in] barycenter Barycenter to subtract to this
    /// \return The subtraction of the two barycenter
    Barycenter  operator- (const Barycenter& barycenter) const;

    /// Subtraction operator overloaded to subtract a part of this barycenter
    /// \param[in] barycenter Barycenter to subtract to this
    /// \return This reduced barycenter
    Barycenter& operator-=(const Barycenter& barycenter);

private :
    Vec3d _center;
    double _mass;
};



// IMPLEMENTATION //
inline const Vec3d& Barycenter::getCenter() const
{
    return _center;
}

inline double Barycenter::getMass() const
{
    return _mass;
}

inline void Barycenter::setCenter(const Vec3d& center)
{
    _center = center;
}

inline void Barycenter::setMass(double mass)
{
    _mass = mass;
}

}

#endif // CELLARWORKBENCH_BARYCENTER_H
