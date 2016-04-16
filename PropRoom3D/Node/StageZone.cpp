#include "StageZone.h"

#include "Node/Prop/Prop.h"
#include "Node/Prop/Surface/Surface.h"
#include "Node/Light/LightBulb/LightBulb.h"
#include "Node/Visitor.h"


namespace prop3
{
    const std::shared_ptr<Surface> StageZone::UNBOUNDED(nullptr);


    StageZone::StageZone(const std::string& name) :
        HandleNode(name),
        _bounds(UNBOUNDED)
    {

    }

    StageZone::~StageZone()
    {
        unregisterFrom(_bounds);

        for(auto prop : _props)
            unregisterFrom(prop);

        for(auto light : _lights)
            unregisterFrom(light);

        for(auto zone : _subzones)
            unregisterFrom(zone);
    }

    void StageZone::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<Node>> StageZone::children() const
    {
        std::vector<std::shared_ptr<Node>> c;

        c.push_back(_bounds);
        c.insert(c.begin(), _props.begin(), _props.end());
        c.insert(c.begin(), _lights.begin(), _lights.end());
        c.insert(c.begin(), _subzones.begin(), _subzones.end());

        return c;
    }

    void StageZone::setBounds(const std::shared_ptr<Surface>& bounds)
    {
        swapChild(_bounds, bounds);
        _bounds = bounds;

        stampCurrentUpdate();
    }

    void StageZone::addProp(const std::shared_ptr<Prop>& prop)
    {
        prop->registerObserver(*this);
        _props.push_back(prop);

        stampCurrentUpdate();
    }

    void StageZone::addLight(const std::shared_ptr<LightBulb>& light)
    {
        light->registerObserver(*this);
        _lights.push_back(light);

        stampCurrentUpdate();
    }

    void StageZone::addSubzone(const std::shared_ptr<StageZone>& zone)
    {
        zone->registerObserver(*this);
        _subzones.push_back(zone);

        stampCurrentUpdate();
    }

    void StageZone::clear()
    {
        unregisterFrom(_bounds);
        _bounds = UNBOUNDED;

        for(auto prop : _props)
            unregisterFrom(prop);
        _props.clear();

        for(auto light : _lights)
            unregisterFrom(light);
        _lights.clear();

        for(auto zone : _subzones)
            unregisterFrom(zone);
        _subzones.clear();

        stampCurrentUpdate();
    }

    void StageZone::transform(const glm::dmat4& mat)
    {
        blockUpdates();

        if(_bounds != UNBOUNDED)
            Surface::transform(_bounds, mat);

        for(const auto& prop : _props)
            prop->transform(mat);

        for(const auto& light : _lights)
            light->transform(mat);

        for(const auto& zone : _subzones)
            zone->transform(mat);

        unblockUpdates();
    }

    void StageZone::translate(const glm::dvec3& dis)
    {
        blockUpdates();

        if(_bounds != UNBOUNDED)
            Surface::translate(_bounds, dis);

        for(const auto& prop : _props)
            prop->translate(dis);

        for(const auto& light : _lights)
            light->translate(dis);

        for(const auto& zone : _subzones)
            zone->translate(dis);

        unblockUpdates();
    }

    void StageZone::rotate(double angle, const glm::dvec3& axis)
    {
        blockUpdates();

        if(_bounds != UNBOUNDED)
            Surface::rotate(_bounds, angle, axis);

        for(const auto& prop : _props)
            prop->rotate(angle, axis);

        for(const auto& light : _lights)
            light->rotate(angle, axis);

        for(const auto& zone : _subzones)
            zone->rotate(angle, axis);

        unblockUpdates();
    }

    void StageZone::scale(double coeff)
    {
        blockUpdates();

        if(_bounds != UNBOUNDED)
            Surface::scale(_bounds, coeff);

        for(const auto& prop : _props)
            prop->scale(coeff);

        for(const auto& light : _lights)
            light->scale(coeff);

        for(const auto& zone : _subzones)
            zone->scale(coeff);

        unblockUpdates();
    }
}
