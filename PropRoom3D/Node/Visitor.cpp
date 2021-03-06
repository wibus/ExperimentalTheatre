#include "Visitor.h"


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

    void Visitor::visit(BoxSideTexture& node)
    {

    }

    void Visitor::visit(BoxBandTexture& node)
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

    void Visitor::visit(Disk& node)
    {

    }


    // Materials
    void Visitor::visit(UniformStdMaterial& node)
    {

    }


    // Coatings
    void Visitor::visit(EmissiveCoating& node)
    {

    }

    void Visitor::visit(UniformStdCoating& node)
    {

    }

    void Visitor::visit(TexturedStdCoating& node)
    {

    }


    // Lights
    void Visitor::visit(CircularLight& node)
    {

    }

    void Visitor::visit(SphericalLight& node)
    {

    }


    // Backdrops
    void Visitor::visit(ProceduralSun& node)
    {

    }
}
