#ifndef PROPROOM3D_HARDWARE_H
#define PROPROOM3D_HARDWARE_H

#include "libPropRoom3D_global.h"
#include <DesignPattern/SpecificSubject.h>


namespace prop3
{
    // Forward declarations
    class Hardware;

    struct PROP3D_EXPORT HardwareUpdate
    {
        enum class EType
        {
            DENSITY,
            BOUNCINESS,
            STATIC_FIRCTION_COEFFICIENT,
            DYNAMIC_FIRCTION_COEFFICIENT,
            NB_TYPES
        };

        HardwareUpdate(EType type, double value, Hardware& material) :
            type(type), value(value), material(material) {}

        EType type;
        double value;
        Hardware& material;
    };

    class PROP3D_EXPORT Hardware : public cellar::SpecificSubject<HardwareUpdate>
    {
    public:
        Hardware();
        virtual ~Hardware();

        virtual double density() const;
        virtual double bounciness() const;
        virtual double staticFrictionCoefficient() const;
        virtual double dynamicFrictionCoefficient() const;

        virtual void setDensity(const double& density);
        virtual void setBounciness(const double& bounciness);
        virtual void setStaticFrictionCoefficient(const double& us);
        virtual void setDynamicFrictionCoefficient(const double& ud);

        // Constant attributes
        static const double INFINITE_DENSITY;

    protected:
        double _density;
        double _staticFrictionCoefficient;
        double _dynamicFrictionCoefficient;
        double _bounciness;
    };
}

#endif // PROPROOM3D_HARDWARE_H
