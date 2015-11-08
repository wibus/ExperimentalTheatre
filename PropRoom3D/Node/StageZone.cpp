#include "StageZone.h"

#include "Prop/Prop.h"
#include "Prop/Surface/Surface.h"
#include "Serial/Visitor.h"


namespace prop3
{
    const std::shared_ptr<Surface> StageZone::UNBOUNDED;


    StageZone::StageZone(const std::string& name) :
        HandleNode(name),
        _bounds(UNBOUNDED)
    {

    }

    StageZone::~StageZone()
    {

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
        c.insert(c.begin(), _subzones.begin(), _subzones.end());

        return c;
    }

    void StageZone::setBounds(const std::shared_ptr<Surface>& bounds)
    {
        _bounds = bounds;

        stampCurrentUpdate();
    }

    void StageZone::addProp(const std::shared_ptr<Prop>& prop)
    {
        _props.push_back(prop);

        stampCurrentUpdate();
    }

    void StageZone::addSubzone(const std::shared_ptr<StageZone>& zone)
    {
        _subzones.push_back(zone);

        stampCurrentUpdate();
    }

    void StageZone::clear()
    {
        _bounds = UNBOUNDED;
        _props.clear();
        _subzones.clear();
    }

    void StageZone::transform(const glm::dmat4& mat)
    {
        if(_bounds != UNBOUNDED)
            Surface::transform(_bounds, mat);

        for(const auto& prop : _props)
            prop->transform(mat);

        for(const auto& zone : _subzones)
            zone->transform(mat);

        stampCurrentUpdate();
    }

    void StageZone::translate(const glm::dvec3& dis)
    {
        if(_bounds != UNBOUNDED)
            Surface::translate(_bounds, dis);

        for(const auto& prop : _props)
            prop->translate(dis);

        for(const auto& zone : _subzones)
            zone->translate(dis);

        stampCurrentUpdate();
    }

    void StageZone::rotate(double angle, const glm::dvec3& axis)
    {
        if(_bounds != UNBOUNDED)
            Surface::rotate(_bounds, angle, axis);

        for(const auto& prop : _props)
            prop->rotate(angle, axis);

        for(const auto& zone : _subzones)
            zone->rotate(angle, axis);

        stampCurrentUpdate();
    }

    void StageZone::scale(double coeff)
    {
        if(_bounds != UNBOUNDED)
            Surface::scale(_bounds, coeff);

        for(const auto& prop : _props)
            prop->scale(coeff);

        for(const auto& zone : _subzones)
            zone->scale(coeff);

        stampCurrentUpdate();
    }
}
