#ifndef PROPROOM3D_EMITTERCOATING_H
#define PROPROOM3D_EMITTERCOATING_H

#include "Coating.h"


namespace prop3
{
    class Sampler;


    class PROP3D_EXPORT EmissiveCoating : public Coating
    {
    public:
        EmissiveCoating(const glm::dvec3& emittedRadiance);
        virtual ~EmissiveCoating();


        // Node interface
        virtual void accept(Visitor& visitor) override;


        virtual glm::dvec4 indirectBrdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report) const override;

        virtual glm::dvec4 directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection) const override;

        virtual glm::dvec3 albedo(
                const RayHitReport& report) const override;


        void setEmittedRadiance(const glm::dvec3& radiance);
        glm::dvec3 emittedRadiance() const;


    private:
        glm::dvec3 _emittedRadiance;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 EmissiveCoating::emittedRadiance() const
    {
        return _emittedRadiance;
    }
}

#endif // PROPROOM3D_EMITTERCOATING_H
