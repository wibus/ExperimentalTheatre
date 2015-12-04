#include "LightBulb.h"

#include "Serial/Visitor.h"
#include "Sampler/Sampler.h"
#include "Prop/Coating/EmissiveCoating.h"


namespace prop3
{
    LightBulb::LightBulb() :
        _sampler(nullptr),
        _coating(new EmissiveCoating())
    {

    }

    LightBulb::~LightBulb()
    {

    }

    void LightBulb::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<Node>> LightBulb::children() const
    {
        return {_sampler, _coating};
    }

    glm::dvec4 LightBulb::raycast(const Raycast& ray, bool directView) const
    {

    }

    std::vector<Raycast> LightBulb::fireRays(unsigned int count) const
    {

    }

    std::vector<Raycast> LightBulb::fireOn(const glm::dvec3& pos, unsigned int count) const
    {

    }

    void LightBulb::setRadiantFlux(double flux)
    {
        _radiantFlux = flux;

        stampCurrentUpdate();
    }

    void LightBulb::setSampler(const std::shared_ptr<Sampler> sampler)
    {
        _sampler = sampler;
        _coating->setSampler(sampler);

        stampCurrentUpdate();
    }

    std::shared_ptr<Coating> LightBulb::coating() const
    {
        return std::static_pointer_cast<Coating>(_coating);
    }
}
