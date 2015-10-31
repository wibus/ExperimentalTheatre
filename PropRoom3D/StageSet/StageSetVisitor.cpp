#include "StageSetVisitor.h"


namespace prop3
{
    StageSetVisitor::StageSetVisitor()
    {

    }

    StageSetVisitor::~StageSetVisitor()
    {

    }


    // Props
    void StageSetVisitor::visit(Prop &node)
    {
    }


    // Implicit Surfaces
    void StageSetVisitor::visit(SurfaceShell& node)
    {

    }

    void StageSetVisitor::visit(SurfaceGhost& node)
    {

    }

    void StageSetVisitor::visit(SurfaceInverse& node)
    {

    }

    void StageSetVisitor::visit(SurfaceOr& node)
    {

    }

    void StageSetVisitor::visit(SurfaceAnd& node)
    {

    }

    void StageSetVisitor::visit(Box& node)
    {

    }

    void StageSetVisitor::visit(BoxTexture& node)
    {

    }

    void StageSetVisitor::visit(Plane& node)
    {

    }

    void StageSetVisitor::visit(PlaneTexture& node)
    {

    }

    void StageSetVisitor::visit(Quadric& node)
    {

    }

    void StageSetVisitor::visit(Sphere& node)
    {

    }


    // Materials
    void StageSetVisitor::visit(UniformStdMaterial& node)
    {

    }


    // Coatings
    void StageSetVisitor::visit(UniformStdCoating& node)
    {

    }

    void StageSetVisitor::visit(TexturedStdCoating& node)
    {

    }


    // Environments
    void StageSetVisitor::visit(Environment& node)
    {

    }


    // Backdrops
    void StageSetVisitor::visit(ProceduralSun& node)
    {

    }
}
