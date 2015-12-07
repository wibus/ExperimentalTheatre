#ifndef PROPROOM3D_LIGHTBULB_H
#define PROPROOM3D_LIGHTBULB_H

#include <memory>

#include "Light.h"


namespace prop3
{
    class Sampler;
    class Coating;
    class EmissiveCoating;


    class PROP3D_EXPORT LightBulb : public Light
    {
    public:
        LightBulb();

        virtual ~LightBulb();


        virtual void accept(Visitor& visito) override;

        virtual std::vector<std::shared_ptr<Node>> children() const override;


        virtual std::vector<Raycast> fireRays(unsigned int count) const override;

        virtual std::vector<Raycast> fireOn(const glm::dvec3& pos, unsigned int count) const override;


        void setRadiantFlux(const glm::dvec3& radiantFlux);
        glm::dvec3 radiantFlux() const;

        void setSampler(const std::shared_ptr<Sampler>& sampler);
        std::shared_ptr<Sampler> sampler() const;

        std::shared_ptr<Coating> coating() const;


    private:
        glm::dvec3 _radiantFlux;
        std::shared_ptr<Sampler> _sampler;
        std::shared_ptr<EmissiveCoating> _coating;
    };



    // IMPLEMENTATION //
    inline glm::dvec3 LightBulb::radiantFlux() const
    {
        return _radiantFlux;
    }

    inline std::shared_ptr<Sampler> LightBulb::sampler() const
    {
        return _sampler;
    }
}

#endif // PROPROOM3D_LIGHT_H
