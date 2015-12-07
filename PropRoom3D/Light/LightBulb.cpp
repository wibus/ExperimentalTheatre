#include "LightBulb.h"

#include "Ray/Raycast.h"
#include "Node/Visitor.h"
#include "Sampler/Sampler.h"
#include "Prop/Coating/EmissiveCoating.h"


namespace prop3
{
    LightBulb::LightBulb() :
        _radiantFlux(0.0),
        _sampler(nullptr),
        _coating(new EmissiveCoating(glm::dvec3(0.0)))
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
        return { _sampler, _coating };
    }

    std::vector<Raycast> LightBulb::fireRays(unsigned int count) const
    {

    }

    std::vector<Raycast> LightBulb::fireOn(const glm::dvec3& pos, unsigned int count) const
    {
    }

    void LightBulb::setRadiantFlux(const glm::dvec3& radiantFlux)
    {
        _radiantFlux = radiantFlux;
        if(_sampler.get() != nullptr)
        {
            _coating->setEmittedRadiance(
                _radiantFlux / _sampler->area());
        }
        else
        {
            _coating->setEmittedRadiance(radiantFlux);
        }

        stampCurrentUpdate();
    }

    void LightBulb::setSampler(const std::shared_ptr<Sampler>& sampler)
    {
        _sampler = sampler;

        if(_sampler.get() != nullptr)
        {
            double area = _sampler->area();
            _coating->setEmittedRadiance(
                _radiantFlux / area);
        }

        stampCurrentUpdate();
    }

    std::shared_ptr<Coating> LightBulb::coating() const
    {
        return _coating;
    }
}
