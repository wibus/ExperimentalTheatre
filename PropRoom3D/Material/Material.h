#ifndef PROPROOM3D_MATERIAL_H
#define PROPROOM3D_MATERIAL_H

#include "libPropRoom3D_global.h"
#include <DesignPattern/SpecificSubject.h>


namespace prop3
{
    // Forward declarations
    class Material;

    struct PROP3D_EXPORT MaterialUpdate
    {
        enum class EType
        {
            DENSITY,
            BOUNCINESS,
            STATIC_FIRCTION_COEFFICIENT,
            DYNAMIC_FIRCTION_COEFFICIENT,
            NB_TYPES
        };

        MaterialUpdate(EType type, double value, Material& material) :
            type(type), value(value), material(material) {}

        EType type;
        double value;
        Material& material;
    };

    class PROP3D_EXPORT Material : public cellar::SpecificSubject<MaterialUpdate>
    {
    public:
        Material();
        virtual ~Material();

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

#endif // PROPROOM3D_MATERIAL_H
