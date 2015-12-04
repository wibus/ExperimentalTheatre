#ifndef PROPROOM3D_EMITTERCOATING_H
#define PROPROOM3D_EMITTERCOATING_H

#include "Coating.h"


namespace prop3
{
    class Sampler;


    class PROP3D_EXPORT EmissiveCoating : public Coating
    {
    public:
        EmissiveCoating();
        virtual ~EmissiveCoating();


        // Node interface
        virtual void accept(Visitor& visitor) override;


        virtual glm::dvec4 indirectBrdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const override;

        virtual glm::dvec4 directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const override;

        virtual glm::dvec3 albedo(
                const RayHitReport& report) const override;


        void setEmittedRadiance(const glm::dvec3& radiance);
        glm::dvec3 emittedRadiance() const;

        void setSampler(const std::shared_ptr<Sampler>& sampler);
        std::shared_ptr<Sampler> sampler() const;


    private:
        glm::dvec3 _emittedRadiance;
        std::shared_ptr<Sampler> _sampler;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 EmissiveCoating::emittedRadiance() const
    {
        return _emittedRadiance;
    }

    inline std::shared_ptr<Sampler> EmissiveCoating::sampler() const
    {
        return _sampler;
    }
}

#endif // PROPROOM3D_EMITTERCOATING_H
