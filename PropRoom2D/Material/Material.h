#ifndef PROPROOM2D_MATERIAL_H
#define PROPROOM2D_MATERIAL_H

#include "libPropRoom2D_global.h"
#include <DesignPattern/SpecificSubject.h>


namespace prop2
{
    // Forward declarations
    class Material;

    struct PROP2D_EXPORT MaterialUpdate
    {
        enum class EType
        {
            DENSITY,
            BOUNCINESS,
            STATIC_FIRCTION_COEFFICIENT,
            DYNAMIC_FIRCTION_COEFFICIENT,
            NB_TYPES
        };

        MaterialUpdate(EType type, real value, Material& material) :
            type(type), value(value), material(material) {}

        EType type;
        real value;
        Material& material;
    };

    class PROP2D_EXPORT Material : public cellar::SpecificSubject<MaterialUpdate>
    {
    public:
        Material();
        virtual ~Material();

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
