#include "StdDesigner.h"

#include <cassert>

#include "../../Prop/Prop.h"
#include "../../Environment/Environment.h"


namespace prop3
{
    StdDesigner::StdDesigner()
    {
    }

    StdDesigner::~StdDesigner()
    {
    }

    void StdDesigner::setup()
    {
    }

    void StdDesigner::reset()
    {
    }

    std::shared_ptr<Prop> StdDesigner::createProp()
    {
        return std::shared_ptr<Prop>(new Prop());
    }

    std::shared_ptr<Environment> StdDesigner::createEnvironment()
    {
        return std::shared_ptr<Environment>(new Environment());
    }
}
