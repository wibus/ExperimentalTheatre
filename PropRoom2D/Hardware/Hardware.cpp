#include "Hardware.h"


namespace prop2
{
    const real Hardware::INFINITE_DENSITY = real(0);


    Hardware::Hardware() :
        _density(real(1)),
        _bounciness(real(1)),
        _staticFrictionCoefficient(real(1)),
        _dynamicFrictionCoefficient(real(1))
    {

    }

    Hardware::~Hardware()
    {

    }

    inline real Hardware::density() const
    {
        return _density;
    }

    inline real Hardware::bounciness() const
    {
        return _bounciness;
    }

    inline real Hardware::staticFrictionCoefficient() const
    {
        return _staticFrictionCoefficient;
    }

    inline real Hardware::dynamicFrictionCoefficient() const
    {
        return _dynamicFrictionCoefficient;
    }

    inline void Hardware::setDensity(const real& density)
    {
        if(_density != density)
        {
            _density = density;
            setIsChanged(true);
            HardwareUpdate msg(
                HardwareUpdate::EType::DENSITY,
                _density,
                *this);
            notifyObservers(msg);
        }
    }

    inline void Hardware::setBounciness(const real& bounciness)
    {
        if(_bounciness != bounciness)
        {
            _bounciness = bounciness;
            setIsChanged(true);
            HardwareUpdate msg(
                HardwareUpdate::EType::BOUNCINESS,
                _bounciness,
                *this);
            notifyObservers(msg);
        }
    }

    inline void Hardware::setStaticFrictionCoefficient(const real& us)
    {
        if(_staticFrictionCoefficient != us)
        {
            _staticFrictionCoefficient = us;
            setIsChanged(true);
            HardwareUpdate msg(
                HardwareUpdate::EType::STATIC_FIRCTION_COEFFICIENT,
                _staticFrictionCoefficient,
                *this);
            notifyObservers(msg);
        }
    }

    inline void Hardware::setDynamicFrictionCoefficient(const real& ud)
    {
        if(_dynamicFrictionCoefficient != ud)
        {
            _dynamicFrictionCoefficient = ud;
            setIsChanged(true);
            HardwareUpdate msg(
                HardwareUpdate::EType::DYNAMIC_FIRCTION_COEFFICIENT,
                _dynamicFrictionCoefficient,
                *this);
            notifyObservers(msg);
        }
    }
}
