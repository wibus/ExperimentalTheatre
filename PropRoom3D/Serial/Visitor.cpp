#include "Serial/Visitor.h"


namespace prop3
{
    Visitor::Visitor()
    {

    }

    Visitor::~Visitor()
    {

    }


    // Stage
    void Visitor::visit(StageSet& node)
    {
    }

    void Visitor::visit(StageZone& node)
    {
    }


    // Props
    void Visitor::visit(Prop &node)
    {
    }


    // Surfaces
    void Visitor::visit(Bounds& node)
    {

    }


    // Implicit Surfaces
    void Visitor::visit(SurfaceShell& node)
    {

    }

    void Visitor::visit(SurfaceGhost& node)
    {

    }

    void Visitor::visit(SurfaceInverse& node)
    {

    }

    void Visitor::visit(SurfaceOr& node)
    {

    }

    void Visitor::visit(SurfaceAnd& node)
    {

    }

    void Visitor::visit(Box& node)
    {

    }

    void Visitor::visit(BoxTexture& node)
    {

    }

    void Visitor::visit(Plane& node)
    {

    }

    void Visitor::visit(PlaneTexture& node)
    {

    }

    void Visitor::visit(Quadric& node)
    {

    }

    void Visitor::visit(Sphere& node)
    {

    }


    // Materials
    void Visitor::visit(UniformStdMaterial& node)
    {

    }


    // Coatings
    void Visitor::visit(UniformStdCoating& node)
    {

    }

    void Visitor::visit(TexturedStdCoating& node)
    {

    }


    // Environments
    void Visitor::visit(Environment& node)
    {

    }


    // Backdrops
    void Visitor::visit(ProceduralSun& node)
    {

    }
}
