#include "Prop.h"

#include "Surface/Surface.h"
#include "Serial/Visitor.h"


namespace prop3
{
    Prop::Prop(const std::string& name) :
        HandleNode(name)
    {
    }

    Prop::~Prop()
    {
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
        _surfaces.push_back(surface);

        stampCurrentUpdate();
    }

    void Prop::clear()
    {
        _surfaces.clear();
    }

    void Prop::transform(const glm::dmat4& mat)
    {
        for(auto& surf : _surfaces)
            Surface::transform(surf, mat);

        stampCurrentUpdate();
    }

    void Prop::translate(const glm::dvec3& dis)
    {
        for(auto& surf : _surfaces)
            Surface::translate(surf, dis);

        stampCurrentUpdate();
    }

    void Prop::rotate(double angle, const glm::dvec3& axis)
    {
        for(auto& surf : _surfaces)
            Surface::rotate(surf, angle, axis);

        stampCurrentUpdate();
    }

    void Prop::scale(double coeff)
    {
        for(auto& surf : _surfaces)
            Surface::scale(surf, coeff);

        stampCurrentUpdate();
    }
}
