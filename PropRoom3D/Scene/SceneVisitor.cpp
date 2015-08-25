#include "SceneVisitor.h"


namespace prop3
{
    SceneVisitor::SceneVisitor()
    {

    }

    SceneVisitor::~SceneVisitor()
    {

    }


    // Props
    void SceneVisitor::visit(Prop &node)
    {
    }


    // Implicit Surfaces
    void SceneVisitor::visit(SurfaceGhost& node)
    {

    }

    void SceneVisitor::visit(SurfaceInverse& node)
    {

    }

    void SceneVisitor::visit(SurfaceOr& node)
    {

    }

    void SceneVisitor::visit(SurfaceAnd& node)
    {

    }

    void SceneVisitor::visit(Plane& node)
    {

    }

    void SceneVisitor::visit(PlaneTexture& node)
    {

    }

    void SceneVisitor::visit(Quadric& node)
    {

    }

    void SceneVisitor::visit(Sphere& node)
    {

    }


    // Materials
    void SceneVisitor::visit(Air& node)
    {

    }

    void SceneVisitor::visit(Fog& node)
    {

    }

    void SceneVisitor::visit(Concrete& node)
    {

    }

    void SceneVisitor::visit(Glass& node)
    {

    }

    void SceneVisitor::visit(Metal& node)
    {

    }


    // Coatings
    void SceneVisitor::visit(NoCoating& node)
    {

    }

    void SceneVisitor::visit(FlatPaint& node)
    {

    }

    void SceneVisitor::visit(GlossyPaint& node)
    {

    }

    void SceneVisitor::visit(TexturedFlatPaint& node)
    {

    }

    void SceneVisitor::visit(TexturedGlossyPaint& node)
    {

    }


    // Environments
    void SceneVisitor::visit(Environment& node)
    {

    }


    // Backdrops
    void SceneVisitor::visit(ProceduralSun& node)
    {

    }
}
