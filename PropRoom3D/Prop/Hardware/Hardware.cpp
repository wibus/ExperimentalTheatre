#include "Hardware.h"


namespace prop3
{
    const double Hardware::INFINITE_DENSITY = 0.0;


    Hardware::Hardware() :
        _density(1.0),
        _bounciness(1.0),
        _staticFrictionCoefficient(1.0),
        _dynamicFrictionCoefficient(1.0)
    {

    }

    Hardware::~Hardware()
    {

    }

    inline double Hardware::density() const
    {
        return _density;
    }

    inline double Hardware::bounciness() const
    {
        return _bounciness;
    }

    inline double Hardware::staticFrictionCoefficient() const
    {
        return _staticFrictionCoefficient;
    }

    inline double Hardware::dynamicFrictionCoefficient() const
    {
        return _dynamicFrictionCoefficient;
    }

    inline void Hardware::setDensity(const double& density)
    {
        if(_density != density)
        {
            _density = density;
            HardwareUpdate msg(
                HardwareUpdate::EType::DENSITY,
                _density,
                *this);
            notifyObservers(msg);
        }
    }

    inline void Hardware::setBounciness(const double& bounciness)
    {
        if(_bounciness != bounciness)
        {
            _bounciness = bounciness;
            HardwareUpdate msg(
                HardwareUpdate::EType::BOUNCINESS,
                _bounciness,
                *this);
            notifyObservers(msg);
        }
    }

    inline void Hardware::setStaticFrictionCoefficient(const double& us)
    {
        if(_staticFrictionCoefficient != us)
        {
            _staticFrictionCoefficient = us;
            HardwareUpdate msg(
                HardwareUpdate::EType::STATIC_FIRCTION_COEFFICIENT,
                _staticFrictionCoefficient,
                *this);
            notifyObservers(msg);
        }
    }

    inline void Hardware::setDynamicFrictionCoefficient(const double& ud)
    {
        if(_dynamicFrictionCoefficient != ud)
        {
            _dynamicFrictionCoefficient = ud;
            HardwareUpdate msg(
                HardwareUpdate::EType::DYNAMIC_FIRCTION_COEFFICIENT,
                _dynamicFrictionCoefficient,
                *this);
            notifyObservers(msg);
        }
    }
}
