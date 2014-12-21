#include "StdDesigner.h"
#include "Prop/Prop.h"

#include <cassert>


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
