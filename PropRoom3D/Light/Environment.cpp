#include "Environment.h"

#include "Backdrop/Backdrop.h"
#include "Prop/Material/Material.h"
#include "Node/Visitor.h"


namespace prop3
{

    Environment::Environment() :
        _backdrop(nullptr),
        _ambientMaterial(material::AIR)
    {

    }

    Environment::~Environment()
    {

    }

    void Environment::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<Node>> Environment::children() const
    {
        return { _backdrop, _ambientMaterial };
    }

    void Environment::setBackdrop(const std::shared_ptr<Backdrop>& backdrop)
    {
        _backdrop = backdrop;

        stampCurrentUpdate();
    }

    void Environment::setAmbientMaterial(const std::shared_ptr<Material>& ambientMaterial)
    {
        _ambientMaterial = ambientMaterial;

        stampCurrentUpdate();
    }
}
