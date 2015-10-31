#include "Environment.h"

#include "Backdrop/Backdrop.h"
#include "Prop/Material/Material.h"
#include "StageSet/StageSetVisitor.h"

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

    void Environment::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<StageSetNode>> Environment::children() const
    {
        return { _backdrop, _ambientMaterial };
    }
}
