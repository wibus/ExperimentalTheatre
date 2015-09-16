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
    void StageSetVisitor::visit(Air& node)
    {

    }

    void StageSetVisitor::visit(Fog& node)
    {

    }

    void StageSetVisitor::visit(Concrete& node)
    {

    }

    void StageSetVisitor::visit(Glass& node)
    {

    }

    void StageSetVisitor::visit(Metal& node)
    {

    }


    // Coatings
    void StageSetVisitor::visit(NoCoating& node)
    {

    }

    void StageSetVisitor::visit(FlatPaint& node)
    {

    }

    void StageSetVisitor::visit(GlossyPaint& node)
    {

    }

    void StageSetVisitor::visit(TexturedFlatPaint& node)
    {

    }

    void StageSetVisitor::visit(TexturedGlossyPaint& node)
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
