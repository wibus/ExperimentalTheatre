#ifndef PROPROOM3D_STDMATERIAL_H
#define PROPROOM3D_STDMATERIAL_H

#include <random>

#include "Material.h"


namespace prop3
{
    class PROP3D_EXPORT StdMaterial : public Material
    {
    protected:
        StdMaterial();

    public:
        virtual ~StdMaterial();


        virtual double lightFreePathLength(
            const Raycast& ray) const override;

        virtual glm::dvec3 lightAttenuation(
            const Raycast& ray) const override;

        virtual void scatterLight(
            std::vector<Raycast>& raycasts,
            const Raycast& ray) const override;


    protected:
        // For exponential distribution
        mutable std::mt19937 _randomEngine;
    };
}

#endif // PROPROOM3D_STDMATERIAL_H
