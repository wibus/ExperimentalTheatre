#include "StdDesigner.h"

#include <cassert>

#include "../../Prop/Prop.h"


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
}
