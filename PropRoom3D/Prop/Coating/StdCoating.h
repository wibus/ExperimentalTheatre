#ifndef PROPROOM3D_STDCOATING_H
#define PROPROOM3D_STDCOATING_H

#include "Coating.h"


namespace prop3
{
    class PROP3D_EXPORT StdCoating : public Coating
    {
    protected:
        StdCoating();

    public:
        virtual ~StdCoating();

        virtual glm::dvec4 indirectBrdf(
                std::vector<Raycast>& raycasts,
                const RayHitReport& report,
                const Raycast& incidentRay) const override;

        virtual glm::dvec4 directBrdf(
                const LightCast& lightCast,
                const RayHitReport& report,
                const Raycast& eyeRay) const override;

        virtual glm::dvec3 albedo(
                const RayHitReport& report) const override;


        virtual double roughness(const glm::dvec3& tex) const = 0;

        virtual glm::dvec4 paintColor(const glm::dvec3& tex) const = 0;

        virtual double paintRefractiveIndex(const glm::dvec3& tex) const = 0;


    protected:
        static glm::dvec3 getMicrofacetNormal(
                const glm::dvec3& wallNormal,
                const glm::dvec3& incidentDir,
                double rough);
    };
}

#endif // PROPROOM3D_STDCOATING_H
