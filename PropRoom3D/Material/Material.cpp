#include "Material.h"


namespace prop3
{
    const double Material::INFINITE_DENSITY = 0.0;


    Material::Material() :
        _density(1.0),
        _bounciness(1.0),
        _staticFrictionCoefficient(1.0),
        _dynamicFrictionCoefficient(1.0)
    {

    }

    Material::~Material()
    {

    }

    inline double Material::density() const
    {
        return _density;
    }

    inline double Material::bounciness() const
    {
        return _bounciness;
    }

    inline double Material::staticFrictionCoefficient() const
    {
        return _staticFrictionCoefficient;
    }

    inline double Material::dynamicFrictionCoefficient() const
    {
        return _dynamicFrictionCoefficient;
    }

    inline void Material::setDensity(const double& density)
    {
        if(_density != density)
        {
            _density = density;
            setIsChanged(true);
            MaterialUpdate msg(
                MaterialUpdate::EType::DENSITY,
                _density,
                *this);
            notifyObservers(msg);
        }
    }

    inline void Material::setBounciness(const double& bounciness)
    {
        if(_bounciness != bounciness)
        {
            _bounciness = bounciness;
            setIsChanged(true);
            MaterialUpdate msg(
                MaterialUpdate::EType::BOUNCINESS,
                _bounciness,
                *this);
            notifyObservers(msg);
        }
    }

    inline void Material::setStaticFrictionCoefficient(const double& us)
    {
        if(_staticFrictionCoefficient != us)
        {
            _staticFrictionCoefficient = us;
            setIsChanged(true);
            MaterialUpdate msg(
                MaterialUpdate::EType::STATIC_FIRCTION_COEFFICIENT,
                _staticFrictionCoefficient,
                *this);
            notifyObservers(msg);
        }
    }

    inline void Material::setDynamicFrictionCoefficient(const double& ud)
    {
        if(_dynamicFrictionCoefficient != ud)
        {
            _dynamicFrictionCoefficient = ud;
            setIsChanged(true);
            MaterialUpdate msg(
                MaterialUpdate::EType::DYNAMIC_FIRCTION_COEFFICIENT,
                _dynamicFrictionCoefficient,
                *this);
            notifyObservers(msg);
        }
    }
}
