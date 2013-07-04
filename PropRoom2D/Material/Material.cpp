#include "Material.h"


namespace prop2
{
    const real Material::INFINITE_DENSITY = real(0);


    Material::Material() :
        _density(real(1)),
        _bounciness(real(1)),
        _staticFrictionCoefficient(real(1)),
        _dynamicFrictionCoefficient(real(1))
    {

    }

    Material::~Material()
    {

    }

    inline real Material::density() const
    {
        return _density;
    }

    inline real Material::bounciness() const
    {
        return _bounciness;
    }

    inline real Material::staticFrictionCoefficient() const
    {
        return _staticFrictionCoefficient;
    }

    inline real Material::dynamicFrictionCoefficient() const
    {
        return _dynamicFrictionCoefficient;
    }

    inline void Material::setDensity(const real& density)
    {
        if(_density != density)
        {
            _density = density;
            setIsChanged(true);
            MaterialUpdate msg(
                MaterialUpdate::DENSITY,
                _density,
                *this);
            notifyObservers(msg);
        }
    }

    inline void Material::setBounciness(const real& bounciness)
    {
        if(_bounciness != bounciness)
        {
            _bounciness = bounciness;
            setIsChanged(true);
            MaterialUpdate msg(
                MaterialUpdate::BOUNCINESS,
                _bounciness,
                *this);
            notifyObservers(msg);
        }
    }

    inline void Material::setStaticFrictionCoefficient(const real& us)
    {
        if(_staticFrictionCoefficient != us)
        {
            _staticFrictionCoefficient = us;
            setIsChanged(true);
            MaterialUpdate msg(
                MaterialUpdate::STATIC_FIRCTION_COEFFICIENT,
                _staticFrictionCoefficient,
                *this);
            notifyObservers(msg);
        }
    }

    inline void Material::setDynamicFrictionCoefficient(const real& ud)
    {
        if(_dynamicFrictionCoefficient != ud)
        {
            _dynamicFrictionCoefficient = ud;
            setIsChanged(true);
            MaterialUpdate msg(
                MaterialUpdate::DYNAMIC_FIRCTION_COEFFICIENT,
                _dynamicFrictionCoefficient,
                *this);
            notifyObservers(msg);
        }
    }
}
