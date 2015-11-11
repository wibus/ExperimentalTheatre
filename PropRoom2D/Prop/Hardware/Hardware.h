#ifndef PROPROOM2D_HARDWARE_H
#define PROPROOM2D_HARDWARE_H

#include <CellarWorkbench/DesignPattern/SpecificSubject.h>

#include "../../libPropRoom2D_global.h"


namespace prop2
{
    // Forward declarations
    class Hardware;

    class PROP2D_EXPORT HardwareUpdate
    {
	public:
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

    class PROP2D_EXPORT Hardware : public cellar::SpecificSubject<HardwareUpdate>
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

#endif // PROPROOM2D_HARDWARE_H
