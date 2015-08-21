#include "Environment.h"

#include "Backdrop/Backdrop.h"
#include "Scene/SceneVisitor.h"

namespace prop3
{

    Environment::Environment() :
        _backdrop(nullptr)
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

    void Environment::accept(SceneVisitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<SceneNode>> Environment::children() const
    {
        return { _backdrop };
    }
}
