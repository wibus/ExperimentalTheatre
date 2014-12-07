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

    std::shared_ptr<Mesh> StdPropDesigner::createMesh(int triangleCount)
    {
        assert(false /* TODO StdPropDesigner::createMesh() */);
        return std::shared_ptr<Mesh>();
    }

    std::shared_ptr<Plane> StdPropDesigner::createPlane()
    {
        assert(false /* TODO StdPropDesigner::createPlane() */);
        return std::shared_ptr<Plane>();
    }

    std::shared_ptr<Sphere> StdPropDesigner::createSphere()
    {
        assert(false /* TODO StdPropDesigner::createSphere() */);
        return std::shared_ptr<Sphere>();
    }
}
