#include "Prop.h"

#include "Surface/Surface.h"
#include "Node/Visitor.h"


namespace prop3
{
    Prop::Prop(const std::string& name) :
        HandleNode(name)
    {
    }

    Prop::~Prop()
    {
        for(auto surf : _surfaces)
            unregisterFrom(surf);
    }

    void Prop::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<Node>> Prop::children() const
    {
        return std::vector<std::shared_ptr<Node>>(
            _surfaces.begin(), _surfaces.end());
    }

    void Prop::addSurface(const std::shared_ptr<Surface>& surface)
    {
        registerTo(surface);
        _surfaces.push_back(surface);

        stampCurrentUpdate();
    }

    void Prop::clear()
    {
        for(auto surf : _surfaces)
            unregisterFrom(surf);
        _surfaces.clear();

        stampCurrentUpdate();
    }

    void Prop::transform(const glm::dmat4& mat)
    {
        blockUpdates();

        for(auto& surf : _surfaces)
            Surface::transform(surf, mat);

        unblockUpdates();
    }

    void Prop::translate(const glm::dvec3& dis)
    {
        blockUpdates();

        for(auto& surf : _surfaces)
            Surface::translate(surf, dis);

        unblockUpdates();
    }

    void Prop::rotate(double angle, const glm::dvec3& axis)
    {
        blockUpdates();

        for(auto& surf : _surfaces)
            Surface::rotate(surf, angle, axis);

        unblockUpdates();
    }

    void Prop::scale(double coeff)
    {
        blockUpdates();

        for(auto& surf : _surfaces)
            Surface::scale(surf, coeff);

        unblockUpdates();
    }
}
