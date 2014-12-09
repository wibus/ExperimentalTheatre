#include "StdPropDesigner.h"

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

    std::shared_ptr<Brep> StdPropDesigner::createBrep()
    {
        assert(false /* TODO StdPropDesigner::createBrep() */);
        return std::shared_ptr<Brep>();
    }

    std::shared_ptr<Mesh> StdPropDesigner::createMesh(int vertexCount)
    {
        assert(false /* TODO StdPropDesigner::createMesh() */);
        return std::shared_ptr<Mesh>();
    }
}
