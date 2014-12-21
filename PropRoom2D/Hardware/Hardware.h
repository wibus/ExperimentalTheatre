#ifndef PROPROOM2D_MATERIAL_H
#define PROPROOM2D_MATERIAL_H

#include <CellarWorkbench/DesignPattern/SpecificSubject.h>

#include "../libPropRoom2D_global.h"


namespace prop2
{
    // Forward declarations
    class Hardware;

    struct PROP2D_EXPORT HardwareUpdate
    {
        enum class EType
        {
            DENSITY,
            BOUNCINESS,
            STATIC_FIRCTION_COEFFICIENT,
            DYNAMIC_FIRCTION_COEFFICIENT,
            NB_TYPES
        };

        HardwareUpdate(EType type, real value, Hardware& material) :
            type(type), value(value), material(material) {}

        EType type;
        real value;
        Hardware& material;
    };

    class PROP2D_EXPORT Hardware : public cellar::SpecificSubject<HardwareUpdate>
    {
    public:
        Hardware();
        virtual ~Hardware();

        virtual real density() const;
        virtual real bounciness() const;
        virtual real staticFrictionCoefficient() const;
        virtual real dynamicFrictionCoefficient() const;

        virtual void setDensity(const real& density);
        virtual void setBounciness(const real& bounciness);
        virtual void setStaticFrictionCoefficient(const real& us);
        virtual void setDynamicFrictionCoefficient(const real& ud);

        // Constant attributes
        static const real INFINITE_DENSITY;

    protected:
        real _density;
        real _staticFrictionCoefficient;
        real _dynamicFrictionCoefficient;
        real _bounciness;
    };
}

#endif // PROPROOM2D_MATERIAL_H
