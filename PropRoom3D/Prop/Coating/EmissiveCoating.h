#ifndef PROPROOM3D_EMITTERCOATING_H
#define PROPROOM3D_EMITTERCOATING_H

#include "Coating.h"


namespace prop3
{
    class Sampler;
    class LightBulb;


    class PROP3D_EXPORT EmissiveCoating : public Coating
    {
    public:
        EmissiveCoating(const LightBulb& lightBulb);
        virtual ~EmissiveCoating();


        // Node interface
        virtual void accept(Visitor& visitor) override;


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


    private:
        const LightBulb& _lightBulb;
    };
}

#endif // PROPROOM3D_EMITTERCOATING_H
