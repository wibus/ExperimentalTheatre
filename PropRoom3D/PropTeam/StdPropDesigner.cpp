#include "StdPropDesigner.h"
#include "Prop/Prop.h"

#include <cassert>


namespace prop3
{
    StdPropDesigner::StdPropDesigner()
    {
    }

    StdPropDesigner::~StdPropDesigner()
    {
    }

    void StdPropDesigner::setup()
    {
    }

    void StdPropDesigner::reset()
    {
    }

    std::shared_ptr<Prop> StdPropDesigner::createProp()
    {
        return std::shared_ptr<Prop>(new Prop());
    }
}
